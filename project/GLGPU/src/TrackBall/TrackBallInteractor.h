#ifndef RSMZ_TRACKBALLINTERACTOR_H
#define RSMZ_TRACKBALLINTERACTOR_H

#include "Camera.h"

#define GLM_FORCE_RADIANS
#include <glm/gtc/quaternion.hpp>

namespace rsmz
{
    class TrackBallInteractor
    {
    public:
        //相机运动类型
        typedef enum CameraMotionType
        {
            NONE, ARC, FIRSTPERSON, PAN, ROLL, ZOOM
        } CameraMotionType;
        static const glm::vec3 X, Y, Z;
    public:
        TrackBallInteractor();
        ~TrackBallInteractor();
    public:
        //计算球心点
        void computePointOnSphere(const glm::vec2& point, glm::vec3& result);
        //计算旋转矩阵
        void computeRotationBetweenVectors(const glm::vec3& start,
                                           const glm::vec3& stop,
                                           glm::quat& result);
        //Camera操作
        Camera* getCamera(void);
        void setCamera(Camera* c);
        CameraMotionType getMotionLeftClick(void);
        CameraMotionType getMotionMiddleClick(void);
        CameraMotionType getMotionRightClick(void);
        CameraMotionType getMotionScroll(void);
        //鼠标操作状态记录
        void setScrollDirection(bool up);
        void setClickPoint(double x, double y);
        void setLeftClicked(bool value);
        void setMiddleClicked(bool value);
        void setMotionLeftClick(CameraMotionType motion);
        void setMotionMiddleClick(CameraMotionType motion);
        void setMotionRightClick(CameraMotionType motion);
        void setMotionScroll(CameraMotionType motion);
        void setRightClicked(bool value);
        //win窗体中WMSIZE的尺寸传入做缩放计算
        void setScreenSize(float width, float height);
        //加速操作系数(加快旋转等)
        void setSpeed(float s);
        //刷新计算结果矩阵 触发更新cam
        void update();
    protected:
        //选中象限
        char clickQuadrant(float x, float y);
        //计算cam点
        void computeCameraEye(glm::vec3& eye);
        //计算世界坐标中的方向
        void computeCameraUp(glm::vec3& up);
        void computePan(glm::vec3& pan);
        void drag();
        void drag(bool isClicked, CameraMotionType motion);
        void dragArc();
        //改变视觉位置和向上方向(世界坐标方向)，同时保持中心点静止
        void dragArcCamera();
        void dragFirstPerson();
        void dragZoom();
        //通过沿着查看眼睛方向移动相机来模拟缩放。
        void dragZoomCamera();
        //使用静态向上和视觉方向移动相机焦点中心位置(操作cam)
        void dragPan();
        //转动视觉方向(这里就是真正的 操作cam)
        void rollCamera();
        //锁定转变
        void freezeTransform();
        void scroll();
        //更新相机视图(操作cam)
        void updateCameraEyeUp(bool eye, bool up);
    private:
        Camera* mCamera;                            //相机
        CameraMotionType mCameraMotionLeftClick;
        CameraMotionType mCameraMotionMiddleClick;
        CameraMotionType mCameraMotionRightClick;
        CameraMotionType mCameraMotionScroll;
        glm::vec2 mClickPoint;                      //选中点
        //鼠标操作状态
        bool mIsDragging;
        bool mIsLeftClick;
        bool mIsMiddleClick;
        bool mIsRightClick;
        bool mIsScrolling;

        float mPanScale;
        glm::vec2 mPrevClickPoint;                  //上次的记录点
        float mRollScale;
        float mRollSum;
        glm::quat mRotation;
        glm::quat mRotationSum;
        float mSpeed;
        glm::vec3 mStartVector;
        glm::vec3 mStopVector;
        float mTranslateLength;
        float mWidth, mHeight;                      //view的size
        float mZoomSum;                             //
        float mZoomScale;                           //
    };
}

#endif




