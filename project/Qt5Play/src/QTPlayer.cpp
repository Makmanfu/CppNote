


#include "stdafx.h"
#include "QTPlayer.h"
#include "DisplayMediaTimer.h"
#include "ImageFilter.h"
#include "Media.h"
#include "ReadPacketsThread.h"

static bool isPressSlider = false;
static bool isPlay = false;

QTPlayer::QTPlayer(QWidget* parent) : QWidget(parent)
{
    ui.setupUi(this);
    QObject::connect(DisplayMediaTimer::getInstance(),//�źŷ����Ķ���
                     SIGNAL(updateFrame(QImage*)),//�ź�
                     ui.openGLWidget,//�۽��յĶ���
                     SLOT(setVideoImage(QImage*))//��
                    );
    QObject::connect(this,//�źŷ����Ķ���
                     SIGNAL(sendPos(float)),//�ź�
                     ReadPacketsThread::getInstance(),//�۽��յĶ���
                     SLOT(receivePos(float))//��
                    );
    setMinimumWidth(400);
    setMinimumHeight(300);
    bottomAnimation = new QPropertyAnimation(ui.bottemWidget, "geometry");
    setMouseTracking(true);
    ui.openGLWidget->setMouseTracking(true);
    popMenu = new QMenu(this);
    leftRightMirrorAction = new QAction(this);
    upDownMirrorAction = new QAction(this);
    rgbAction = new QAction(this);
    grayAction = new QAction(this);
    netAddressAction = new QAction(this);
    leftRightMirrorAction->setText(QString::fromLocal8Bit("���Ҿ���"));
    upDownMirrorAction->setText(QString::fromLocal8Bit("���¾���"));
    rgbAction->setText(QString::fromLocal8Bit("��ɫ"));
    grayAction->setText(QString::fromLocal8Bit("�ڰ�"));
    netAddressAction->setText(QString::fromLocal8Bit("���紮��"));
    connect(leftRightMirrorAction, &QAction::triggered, this, &QTPlayer::mirrorLeftAndRight);
    connect(upDownMirrorAction, &QAction::triggered, this, &QTPlayer::mirrorUpAndDown);
    connect(rgbAction, &QAction::triggered, this, &QTPlayer::gray2Rgb);
    connect(grayAction, &QAction::triggered, this, &QTPlayer::rgb2Gray);
    connect(netAddressAction, &QAction::triggered, this, &QTPlayer::netAddressInput);
    startTimer(40);
    ReadPacketsThread* readPacketsThread = ReadPacketsThread::getInstance();
    readPacketsThread->start();
    Media::getInstance()->video->start();
    play();
}

void QTPlayer::netAddressInput()
{
    bool ok = FALSE;
    QString text = QInputDialog::getText( this,
                                          QString::fromLocal8Bit("������"),
                                          QString::fromLocal8Bit("�������ַ"),
                                          QLineEdit::Normal, QString::null, &ok);
    if (ok && !text.isEmpty())
        openNetAddressVideo(text);
    else
        ;// �û�����Cancel
}

void QTPlayer::openNetAddressVideo(QString address)
{
    std::string file = address.toLocal8Bit().data();//��ֹ������
    Media* media = Media::getInstance()
                   ->setMediaFile(file.c_str())
                   ->config();
    setWindowTitle(address);
    int total = Media::getInstance()->totalMs;
    ui.totalHour->setText(QString::number((int)(Media::getInstance()->totalMs / 1000 / 60 / 60)) + ":");//������Ƶ�ܵ�ʱ����
    ui.totalMinute->setText(QString::number((int)(Media::getInstance()->totalMs / 1000 / 60) % 60) + ":");
    ui.totalSecond->setText(QString::number((int)(Media::getInstance()->totalMs / 1000) % 60 % 60));
    isPlay = false;
    play();
    float pos = 0;
    pos = (float)ui.volumeSlider->value() / (float)(ui.volumeSlider->maximum() + 1);
    if (Media::getInstance()->getAVFormatContext())
    {
        //Media::getInstance()->audio->setVolume(pos * SDL_MIX_MAXVOLUME); //��ʼ������Ϊ���������һ��
        Media::getInstance()->audio->setVolume(SDL_MIX_MAXVOLUME);
    }        
}

void QTPlayer::contextMenuEvent(QContextMenuEvent* event)
{
    popMenu->clear();
    popMenu->addAction(leftRightMirrorAction);
    popMenu->addAction(upDownMirrorAction);
    popMenu->addAction(rgbAction);
    popMenu->addAction(grayAction);
    popMenu->addAction(netAddressAction);
    //�˵����ֵ�λ��Ϊ��ǰ����λ��
    popMenu->exec(QCursor::pos());
    event->accept();
}

void QTPlayer::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (windowState() &  Qt::WindowFullScreen)
        showNormal();
    else
        showFullScreen();
}

void QTPlayer::mouseMoveEvent(QMouseEvent* event)
{
    //���أ���ʾ�ײ�
    //QMoveDlg::mouseMoveEvent(event);
    int y = event->globalY();
    if (y >= height() - 100 || y <= 5)
        showBottomInAnimation();
    else
        hideBottomInAnimation();
}

void QTPlayer::openVideoFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "please select your video file!");
    if (fileName.isEmpty())
        return;
    QStringList titles = fileName.split("/");
    QString postfix = titles.constLast().split(".").last();
    if (postfix != QString::fromLocal8Bit("mp4")
            && postfix != QString::fromLocal8Bit("rmvb")
            && postfix != QString::fromLocal8Bit("flv")
            && postfix != QString::fromLocal8Bit("avi")
            && postfix != QString::fromLocal8Bit("mkv")
       )
        return;
    std::string file = fileName.toLocal8Bit().data();//��ֹ������
    Media* media = Media::getInstance()
                   ->setMediaFile(file.c_str())
                   ->config();
    setWindowTitle(titles.constLast());
    int total = Media::getInstance()->totalMs;
    ui.totalHour->setText(QString::number((int)(Media::getInstance()->totalMs / 1000 / 60 / 60)) + ":");//������Ƶ�ܵ�ʱ����
    ui.totalMinute->setText(QString::number((int)(Media::getInstance()->totalMs / 1000 / 60) % 60) + ":");
    ui.totalSecond->setText(QString::number((int)(Media::getInstance()->totalMs / 1000) % 60 % 60));
    isPlay = false;
    play();
    float pos = 0;
    pos = (float)ui.volumeSlider->value() / (float)(ui.volumeSlider->maximum() + 1);
    if (Media::getInstance()->getAVFormatContext())
        Media::getInstance()->audio->setVolume(pos * SDL_MIX_MAXVOLUME); //��ʼ������Ϊ���������һ��
}

void QTPlayer::setVolume(int volume)
{
    float pos = 0;
    pos = (float)ui.volumeSlider->value() / (float)(ui.volumeSlider->maximum() + 1);
    if (Media::getInstance()->getAVFormatContext())
        Media::getInstance()->audio->setVolume(pos * SDL_MIX_MAXVOLUME);
}

void QTPlayer::showBottomInAnimation()
{
    if (ui.bottemWidget->y() == height())
    {
        bottomAnimation->setDuration(500);
        bottomAnimation->setStartValue(QRect(ui.bottemWidget->x(), height(), ui.bottemWidget->width(), ui.bottemWidget->height()));
        bottomAnimation->setEndValue(QRect(ui.bottemWidget->x(), this->height() - ui.bottemWidget->height(), ui.bottemWidget->width(), ui.bottemWidget->height()));
        bottomAnimation->start();
    }
}

void QTPlayer::hideBottomInAnimation()
{
    if (ui.bottemWidget->y() == this->height() - ui.bottemWidget->height())
    {
        bottomAnimation->setDuration(500);
        bottomAnimation->setStartValue(QRect(ui.bottemWidget->x(), this->height() - ui.bottemWidget->height(), ui.bottemWidget->width(), ui.bottemWidget->height()));
        bottomAnimation->setEndValue(QRect(ui.bottemWidget->x(), this->height(), ui.bottemWidget->width(), ui.bottemWidget->height()));
        bottomAnimation->start();
    }
}

void QTPlayer::timerEvent(QTimerEvent* e)
{
    //���ý������Ͳ���ʱ��
    if (Media::getInstance()->totalMs > 0)
    {
        double pts = (double)Media::getInstance()->pts * 1000;
        double total = (double)Media::getInstance()->totalMs;
        double rate = pts / total;
        if (!isPressSlider && isPlay)
        {
            ui.playslider->setValue(rate);
            ui.currentHour->setText(QString::number((int)(pts / 1000000 / 60 / 60)) + ":");
            ui.currentMinute->setText(QString::number((int)(pts / 1000000 / 60) % 60) + ":");
            ui.currentSecond->setText(QString::number((int)(pts / 1000000) % 60 % 60));
        }
    }
}

void QTPlayer::sliderPress()
{
    //���½�����
    isPressSlider = true;
}

void QTPlayer::sliderRelease()
{
    //�������ͷ�
    isPressSlider = false;
    float pos = 0;
    pos = (float)ui.playslider->value() / (float)(ui.playslider->maximum() + 1);
    sendPos(pos);
}

void QTPlayer::play()
{
    const QString PAUSE = "QPushButton#playButton{border-image:url(:/ffplay/PauseP.png);}";
    const QString PLAY = "QPushButton#playButton{border-image:url(:/ffplay/PlayP.png);}";
    isPlay = !isPlay;
    DisplayMediaTimer::getInstance()->setPlay(isPlay);
    if (isPlay)
    {
        ui.bottemWidget->setStyleSheet(PAUSE);
        if (Media::getInstance()->getAVFormatContext())
            Media::getInstance()->audio->audioPlay();
    }
    else
    {
        ui.bottemWidget->setStyleSheet(PLAY);
        if (Media::getInstance()->getAVFormatContext())
            SDL_CloseAudio();
    }
}

void QTPlayer::mirrorUpAndDown()
{
    if (Media::getInstance()->getAVFormatContext() && isPlay)
        ImageFilter::getInstance()->addTask(XTask{ XTASK_MIRRORUPANDDOWN });
}

void QTPlayer::mirrorLeftAndRight()
{
    if (Media::getInstance()->getAVFormatContext() && isPlay)
        ImageFilter::getInstance()->addTask(XTask{ XTASK_MIRRORLEFTANDRIGHT });
}

void QTPlayer::gray2Rgb()
{
    if (Media::getInstance()->getAVFormatContext() && isPlay)
        ImageFilter::getInstance()->addColorTask(ColorTask{ COLRTASK_GRAY_TO_RGBA });
}

void QTPlayer::rgb2Gray()
{
    if (Media::getInstance()->getAVFormatContext() && isPlay)
    {
        for (int i = 0; i < 5; i++)
            ImageFilter::getInstance()->addColorTask(ColorTask{ COLRTASK_RGBA_TO_GRAY });
    }
}

void QTPlayer::resizeEvent(QResizeEvent* e)
{
    //����ı��Сʱ���������пؼ����Բ��Ű�ťΪ��׼
    ui.openGLWidget->resize(size());
    ui.bottemWidget->resize(this->width(), ui.bottemWidget->height());
    ui.bottemWidget->move(0, this->height() - ui.bottemWidget->height());
    DisplayMediaTimer::getInstance()->resetImage(width(), height());
}