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
        //����˶�����
        typedef enum CameraMotionType
        {
            NONE, ARC, FIRSTPERSON, PAN, ROLL, ZOOM
        } CameraMotionType;
        static const glm::vec3 X, Y, Z;
    public:
        TrackBallInteractor();
        ~TrackBallInteractor();
    public:
        //�������ĵ�
        void computePointOnSphere(const glm::vec2& point, glm::vec3& result);
        //������ת����
        void computeRotationBetweenVectors(const glm::vec3& start,
                                           const glm::vec3& stop,
                                           glm::quat& result);
        //Camera����
        Camera* getCamera(void);
        void setCamera(Camera* c);
        CameraMotionType getMotionLeftClick(void);
        CameraMotionType getMotionMiddleClick(void);
        CameraMotionType getMotionRightClick(void);
        CameraMotionType getMotionScroll(void);
        //������״̬��¼
        void setScrollDirection(bool up);
        void setClickPoint(double x, double y);
        void setLeftClicked(bool value);
        void setMiddleClicked(bool value);
        void setMotionLeftClick(CameraMotionType motion);
        void setMotionMiddleClick(CameraMotionType motion);
        void setMotionRightClick(CameraMotionType motion);
        void setMotionScroll(CameraMotionType motion);
        void setRightClicked(bool value);
        //win������WMSIZE�ĳߴ紫�������ż���
        void setScreenSize(float width, float height);
        //���ٲ���ϵ��(�ӿ���ת��)
        void setSpeed(float s);
        //ˢ�¼��������� ��������cam
        void update();
    protected:
        //ѡ������
        char clickQuadrant(float x, float y);
        //����cam��
        void computeCameraEye(glm::vec3& eye);
        //�������������еķ���
        void computeCameraUp(glm::vec3& up);
        void computePan(glm::vec3& pan);
        void drag();
        void drag(bool isClicked, CameraMotionType motion);
        void dragArc();
        //�ı��Ӿ�λ�ú����Ϸ���(�������귽��)��ͬʱ�������ĵ㾲ֹ
        void dragArcCamera();
        void dragFirstPerson();
        void dragZoom();
        //ͨ�����Ų鿴�۾������ƶ������ģ�����š�
        void dragZoomCamera();
        //ʹ�þ�̬���Ϻ��Ӿ������ƶ������������λ��(����cam)
        void dragPan();
        //ת���Ӿ�����(������������� ����cam)
        void rollCamera();
        //����ת��
        void freezeTransform();
        void scroll();
        //���������ͼ(����cam)
        void updateCameraEyeUp(bool eye, bool up);
    private:
        Camera* mCamera;                            //���
        CameraMotionType mCameraMotionLeftClick;
        CameraMotionType mCameraMotionMiddleClick;
        CameraMotionType mCameraMotionRightClick;
        CameraMotionType mCameraMotionScroll;
        glm::vec2 mClickPoint;                      //ѡ�е�
        //������״̬
        bool mIsDragging;
        bool mIsLeftClick;
        bool mIsMiddleClick;
        bool mIsRightClick;
        bool mIsScrolling;

        float mPanScale;
        glm::vec2 mPrevClickPoint;                  //�ϴεļ�¼��
        float mRollScale;
        float mRollSum;
        glm::quat mRotation;
        glm::quat mRotationSum;
        float mSpeed;
        glm::vec3 mStartVector;
        glm::vec3 mStopVector;
        float mTranslateLength;
        float mWidth, mHeight;                      //view��size
        float mZoomSum;                             //
        float mZoomScale;                           //
    };
}

#endif




