
#include "stdafx.h"
#include "Animator.h"

namespace rsmz
{
    Animator::Animator() :
        mAnimation(NONE),
        mFrame(0),
        mFrames(0),
        mFramesPerSecond(30.),
        mHeight(0),
        mInteractor(0),
        mWidth(0)
    {
        stopwatch();
    }

    Animator::~Animator()
    {
    }

    void Animator::animate(void)
    {
        if (elapsedSeconds() < 1.0 / mFramesPerSecond)
            return;
        switch (mAnimation)
        {
            case FIRST_PERSON:
                firstperson();
                break;
            case ORBIT:
                orbit();
                break;
            case PAN:
                pan();
                break;
            case ROLL:
                roll();
                break;
            case ZOOM:
                zoom();
                break;
            case NONE:
            default:
                return;
                break;
        }
        stopwatch();
    }

    double Animator::elapsedSeconds(void)
    {
        std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
        std::chrono::duration<double> seconds = now - mTic;
        return seconds.count();
    }

    void Animator::firstperson(void)
    {
        //没有写哦
    }

    void Animator::orbit(void)
    {
        //旋转演示动画
        Camera* c = mInteractor->getCamera();
        if (0 == mFrame)
        {
            mFrames = 5 * mFramesPerSecond;
            c->setEye(1., 1., 1.);
            c->setUp(-0., -0., 1.);
            c->setCenter(0, 0, 0);
            c->update();
            mInteractor->setCamera(c);
        }
        double x = fmod(mFrame * 4, mWidth);
        double y = mHeight * .74;
        mInteractor->setLeftClicked(true);
        mInteractor->setClickPoint(x, y);
        if (++mFrame >= mFrames)
        {
            mInteractor->setLeftClicked(false);
            reset();
        }
    }

    void Animator::pan(void)
    {
        //没有写哦
    }

    void Animator::reset(void)
    {
        mAnimation = NONE;
        mFrame = 0;
    }

    void Animator::roll(void)
    {
        //没有写哦
    }

    void Animator::setAnimation(AnimationType type)
    {
        mAnimation = type;
    }

    void Animator::setInteractor(TrackBallInteractor* i)
    {
        mInteractor = i;
    }

    void Animator::setScreenSize(int w, int h)
    {
        mWidth = w;
        mHeight = h;
    }

    void Animator::stopwatch(void)
    {
        mTic = std::chrono::system_clock::now();
    }

    void Animator::zoom(void)
    {
        //放大动画没有写哦
    }
}


