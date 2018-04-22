//*************************************************************************
//
//                   3DVIEW ������ת���ģ��
//                                          @2017-03-16��������
//
//*************************************************************************
#ifndef TRACKBALL_H
#define TRACKBALL_H

#define GLM_ENABLE_EXPERIMENTAL
#include <GL/gl.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GLFW/glfw3.h>

//�����(ת���Ӿ�����Ϳ������)
class CameraEye
{
public:
    CameraEye();
    ~CameraEye();
public:
    const glm::mat4& getMatrix(void);
    const float* getMatrixFlat(void);
    const glm::vec3& getCenter(void);
    const glm::vec3& getEye(void);
    const glm::vec3& getUp(void);
    //�������λ��
    void reset();
    //�������ת�����������ĵ�
    void setCenter(float x, float y, float z);
    void setCenter(const glm::vec3& c);
    //�������λ���ӽǵ�
    void setEye(float x, float y, float z);
    void setEye(const glm::vec3& e);
    //�������������еķ���
    void setUp(float x, float y, float z);
    void setUp(const glm::vec3& u);
    //ˢ�������ͼ
    void update(void);
private:
    glm::vec3 mCenter;          //���ĵ�(�����ͷ��׼�����������������λ��)
    glm::vec3 mEye;             //�ӽǵ�(��������������λ��)
    glm::mat4 mMatrix;          //��ͼ����
    glm::vec3 mUp;              //���������еķ���
};

//GL��ģ�ͻ��Ʋ�������
class RendererGL
{
public:
    RendererGL();
    virtual ~RendererGL();
public:
    void init(void);
    void renderClear(void);
    //���Ƴ������ж���
    virtual void renderObjects(void);
    void render(void);
    void resize(int width, int height);
    void setCamera(CameraEye* c);
    const CameraEye* getCamera(void);
private:
    //����������ο���
    void renderAxes(void);
    //�������ĵ�
    void renderCenter(void);
    //�������ĵ㶥��������
    void renderCube(void);
    //��������Ϊ���ĵ�������
    void renderCubeCenter(void);
    //��������
    void renderGEO(void);
    //���Ƹ�������
    void renderGrid(void);
protected:
    //SIZE�����õ�����
    glm::mat4 mProjection;
    CameraEye* mCamera;
};

//TrackBall(��Ԫ������Ϳ������Ŷ)
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
    CameraEye* getCamera(void);
    void setCamera(CameraEye* c);
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
    CameraEye* mCamera;                            //���
    CameraMotionType mCameraMotionLeftClick;
    CameraMotionType mCameraMotionMiddleClick;
    CameraMotionType mCameraMotionRightClick;
    CameraMotionType mCameraMotionScroll;
    glm::vec2 mClickPoint;                      //ѡ�е�
    //������״̬
    bool mIsDragging, mIsLeftClick, mIsMiddleClick, mIsRightClick, mIsScrolling;

    float mPanScale;
    glm::vec2 mPrevClickPoint;                  //�ϴεļ�¼��
    float mRollScale;
    float mRollSum;
    glm::quat mRotation, mRotationSum;
    float mSpeed;
    glm::vec3 mStartVector, mStopVector;
    float mTranslateLength;
    float mWidth, mHeight;                      //view��size
    float mZoomSum;                             //
    float mZoomScale;                           //
};

/**
Example using GLFW.
Only one window instance is supported, but this could be extended by
using a registry class to scale the static callback handlers.
*/
class RenderWinGLFW
{
public:
    RenderWinGLFW();
    ~RenderWinGLFW();
public:
    //����
    static RenderWinGLFW& instance();
    int run(int width, int height);
    //�ص�����
    static void buttonCallback(GLFWwindow* window, int button, int action, int mods);
    static void errorCallback(int error, const char* description);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void moveCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xpos, double ypos);
    static void sizeCallback(GLFWwindow* window, int width, int height);
private:
    ///Animator mAnimator;                  //���������� ��ʾ
    CameraEye mCamera;                      //���
    TrackBallInteractor mInteractor;        //����TrackBall��ģ��
    RendererGL mRenderer;                   //GL����
    GLFWwindow* mWindow;                    //���ڽ���
};

#endif




