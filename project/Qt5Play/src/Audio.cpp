
#include "stdafx.h"
#include "Audio.h"
#include "Media.h"
#include <fstream>

int Audio::audioVolume = 64;

double Audio::r2d(AVRational r)
{
    return r.num == 0 || r.den == 0 ? 0. : (double)r.num / (double)r.den;
}

void Audio::audioCallback(void* userdata, Uint8* stream, int len)
{
    Audio*  audio = Media::getInstance()->audio;
    SDL_memset(stream, 0, len);
    int audio_size = 0;
    int len1 = 0;
    while (len > 0)// ���豸���ͳ���Ϊlen������
    {
        if (audio->getAudioBuffIndex() >= audio->getAudioBuffSize()) // ��������������
        {
            // ��packet�н�������
            audio_size = audioDecodeFrame(audio, audio->getAudioBuff(), sizeof(audio->getAudioBuff()));
            if (audio_size < 0) // û�н��뵽���ݻ�������0
            {
                audio->setAudioBuffSize(0);
                memset(audio->getAudioBuff(), 0, audio->getAudioBuffSize());
            }
            else
                audio->setAudioBuffSize(audio_size);
            audio->setAudioBuffIndex(0);
        }
        len1 = audio->getAudioBuffSize() - audio->getAudioBuffIndex(); // ��������ʣ�µ����ݳ���
        if (len1 > len) // ���豸���͵����ݳ���Ϊlen
            len1 = len;
        SDL_MixAudio(stream, audio->getAudioBuff() + audio->getAudioBuffIndex(), len, audioVolume);
        len -= len1;
        stream += len1;
        audio->setAudioBuffIndex(audio->getAudioBuffIndex() + len1);
    }
}

int Audio::audioDecodeFrame(Audio* audio, uint8_t* audioBuffer, int bufferSize)
{
    AVFrame* frame = av_frame_alloc();
    int data_size = 0;
    AVPacket pkt = audio->dequeuePacket();
    SwrContext* swr_ctx = nullptr;
    static double clock = 0;
    /*  if (quit)
    return -1;*/
    if (pkt.size <= 0)
        return -1;
    if (pkt.pts != AV_NOPTS_VALUE)
    {
        if (audio->getStream() == nullptr)
            return -1;
        audio->setAudioClock(av_q2d(audio->getStream()->time_base) * pkt.pts);
    }
    if (audio->getAVCodecContext() == nullptr)
        return -1;
    int ret = avcodec_send_packet(audio->getAVCodecContext(), &pkt);
    if (ret < 0 && ret != AVERROR(EAGAIN) && ret != AVERROR_EOF)
        return -1;
    ret = avcodec_receive_frame(audio->getAVCodecContext(), frame);
    if (ret < 0 && ret != AVERROR_EOF)
        return -1;
    int p = (frame->pts * r2d(audio->getStream()->time_base)) * 1000;
    Media::getInstance()->pts = p;
    // ����ͨ������channel_layout
    if (frame->channels > 0 && frame->channel_layout == 0)
        frame->channel_layout = av_get_default_channel_layout(frame->channels);
    else if (frame->channels == 0 && frame->channel_layout > 0)
        frame->channels = av_get_channel_layout_nb_channels(frame->channel_layout);
    AVSampleFormat dst_format = AV_SAMPLE_FMT_S16;//av_get_packed_sample_fmt((AVSampleFormat)frame->format);
    Uint64 dst_layout = av_get_default_channel_layout(frame->channels);
    // ����ת������
    swr_ctx = swr_alloc_set_opts(nullptr, dst_layout, dst_format, frame->sample_rate,
        frame->channel_layout, (AVSampleFormat)frame->format, frame->sample_rate, 0, nullptr);
    if (!swr_ctx || swr_init(swr_ctx) < 0)
        return -1;
    // ����ת�����sample���� a * b / c
    uint64_t dst_nb_samples = av_rescale_rnd(swr_get_delay(swr_ctx, frame->sample_rate) + frame->nb_samples, frame->sample_rate, frame->sample_rate, AVRounding(1));
    // ת��������ֵΪת�����sample����
    int nb = swr_convert(swr_ctx, &audioBuffer, static_cast<int>(dst_nb_samples), (const uint8_t**)frame->data, frame->nb_samples);
    data_size = frame->channels * nb * av_get_bytes_per_sample(dst_format);
    // ÿ������Ƶ���ŵ��ֽ��� sample_rate * channels * sample_format(һ��sampleռ�õ��ֽ���)
    if (audio->getStream()->codec == nullptr)
        return -1;
    audio->setAudioClock(audio->getAudioClock() + static_cast<double>(data_size) / (2 * audio->getStream()->codec->channels * audio->getStream()->codec->sample_rate));
    av_frame_free(&frame);
    swr_free(&swr_ctx);
    return data_size;
}

Audio::Audio()
{
    audioContext = nullptr;
    streamIndex = -1;
    stream = nullptr;
    audioClock = 0;
    audioBuff = new uint8_t[BUFFERSIZE];
    audioBuffSize = 0;
    audioBuffIndex = 0;
}

Audio::~Audio()
{
    audioClose();
    SDL_Quit();
    if (audioBuff)
        delete[] audioBuff;
}

bool Audio::audioClose()
{
    SDL_CloseAudio();
    return true;
}

bool Audio::audioPlay()
{
    SDL_AudioSpec desired;
    desired.freq = audioContext->sample_rate;
    desired.channels = audioContext->channels;
    desired.format = AUDIO_S16SYS;
    desired.samples = 1024;
    desired.silence = 0;
    desired.userdata = this;
    desired.callback = audioCallback;
    if (SDL_OpenAudio(&desired, nullptr) < 0)
        return false;
    SDL_PauseAudio(0); // playing
    return true;
}

double Audio::getCurrentAudioClock()
{
    int hw_buf_size = audioBuffSize - audioBuffIndex;
    int bytes_per_sec = stream->codec->sample_rate * audioContext->channels * 2;
    //int bytes_per_sec = stream->codecpar->sample_rate * audioContext->channels * 2;

    double pts = audioClock - static_cast<double>(hw_buf_size) / bytes_per_sec;
    return pts;
}

int Audio::getStreamIndex()
{
    return streamIndex;
}

void Audio::setStreamIndex(const int streamIndex)
{
    this->streamIndex = streamIndex;
}

int Audio::getAudioQueueSize()
{
    return audiaPackets.getPacketSize();
}

void Audio::enqueuePacket(const AVPacket pkt)
{
    audiaPackets.enQueue(pkt);
}

AVPacket Audio::dequeuePacket()
{
    return audiaPackets.deQueue();
}

uint8_t* Audio::getAudioBuff()
{
    return audioBuff;
}

void Audio::setAudioBuff(uint8_t*&  audioBuff)
{
    this->audioBuff = audioBuff;
}

uint32_t Audio::getAudioBuffSize()
{
    return audioBuffSize;
}

void Audio::setAudioBuffSize(uint32_t audioBuffSize)
{
    this->audioBuffSize = audioBuffSize;
}

uint32_t Audio::getAudioBuffIndex()
{
    return audioBuffIndex;
}

void Audio::setAudioBuffIndex(uint32_t audioBuffIndex)
{
    this->audioBuffIndex = audioBuffIndex;
}

double Audio::getAudioClock()
{
    return audioClock;
}

void Audio::setAudioClock(const double& audioClock)
{
    this->audioClock = audioClock;
}

AVStream* Audio::getStream()
{
    return this->stream;
}

void Audio::setStream(AVStream*& stream)
{
    this->stream = stream;
}

AVCodecContext* Audio::getAVCodecContext()
{
    return this->audioContext;
}

void Audio::setAVCodecContext(AVCodecContext* audioContext)
{
    this->audioContext = audioContext;
}

bool Audio::getIsPlaying()
{
    return isPlay;
}

void Audio::setPlaying(bool isPlaying)
{
    this->isPlay = isPlaying;
}

void Audio::clearPacket()
{
    audiaPackets.queueFlush();
}

void Audio::setVolume(int volume)
{
    audioVolume = volume;
}


