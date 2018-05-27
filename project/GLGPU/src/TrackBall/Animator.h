#ifndef ANIMATOR_H
#define ANIMATOR_H

#include <chrono>
#include "TrackBallInteractor.h"

namespace rsmz
{
    //动画控制器
    class Animator
    {
    public:
        typedef enum AnimationType
        {
            NONE, FIRST_PERSON, ORBIT, PAN, ROLL, ZOOM
        } AnimationType;
    public:
        Animator();
        ~Animator();
    public:
        //动画循环
        void animate(void);
        double elapsedSeconds(void);
        void firstperson(void);
        void orbit(void);
        void pan(void);
        void reset(void);
        void roll(void);
        //设置动画类型
        void setAnimation(AnimationType type);
        void setScreenSize(int w, int h);
        void setInteractor(TrackBallInteractor* i);
        void stopwatch(void);
        void zoom(void);
    private:
        AnimationType mAnimation;
        TrackBallInteractor* mInteractor;
        int mFrame;
        int mFrames;
        float mFramesPerSecond;
        int mHeight;
        std::chrono::time_point<std::chrono::system_clock> mTic;
        int mWidth;
    };
}
#endif


