//*************************************************************************
//
//                   3DVIEW 球心旋转相机模块
//                                          @2017-03-16阿甘整理
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

//相机类(转动视觉画面就靠这个啦)
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
    //重置相机位置
    void reset();
    //设置相机转动的中心球心点
    void setCenter(float x, float y, float z);
    void setCenter(const glm::vec3& c);
    //设置相机位置视角点
    void setEye(float x, float y, float z);
    void setEye(const glm::vec3& e);
    //设置世界坐标中的方向
    void setUp(float x, float y, float z);
    void setUp(const glm::vec3& u);
    //刷新相机视图
    void update(void);
private:
    glm::vec3 mCenter;          //中心点(相机镜头对准的物体在世界坐标的位置)
    glm::vec3 mEye;             //视角点(相机在世界坐标的位置)
    glm::mat4 mMatrix;          //视图矩阵
    glm::vec3 mUp;              //世界坐标中的方向
};

//GL的模型绘制测试数据
class RendererGL
{
public:
    RendererGL();
    virtual ~RendererGL();
public:
    void init(void);
    void renderClear(void);
    //绘制场景所有对象
    virtual void renderObjects(void);
    void render(void);
    void resize(int width, int height);
    void setCamera(CameraEye* c);
    const CameraEye* getCamera(void);
private:
    //绘制坐标轴参考线
    void renderAxes(void);
    //绘制球心点
    void renderCenter(void);
    //绘制球心点顶点正方体
    void renderCube(void);
    //绘制球心为中心点正方体
    void renderCubeCenter(void);
    //绘制球体
    void renderGEO(void);
    //绘制辅助网格
    void renderGrid(void);
protected:
    //SIZE缩放用到矩阵
    glm::mat4 mProjection;
    CameraEye* mCamera;
};

//TrackBall(四元数计算就靠这个了哦)
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
    CameraEye* getCamera(void);
    void setCamera(CameraEye* c);
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
    CameraEye* mCamera;                            //相机
    CameraMotionType mCameraMotionLeftClick;
    CameraMotionType mCameraMotionMiddleClick;
    CameraMotionType mCameraMotionRightClick;
    CameraMotionType mCameraMotionScroll;
    glm::vec2 mClickPoint;                      //选中点
    //鼠标操作状态
    bool mIsDragging, mIsLeftClick, mIsMiddleClick, mIsRightClick, mIsScrolling;

    float mPanScale;
    glm::vec2 mPrevClickPoint;                  //上次的记录点
    float mRollScale;
    float mRollSum;
    glm::quat mRotation, mRotationSum;
    float mSpeed;
    glm::vec3 mStartVector, mStopVector;
    float mTranslateLength;
    float mWidth, mHeight;                      //view的size
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
    //单例
    static RenderWinGLFW& instance();
    int run(int width, int height);
    //回调函数
    static void buttonCallback(GLFWwindow* window, int button, int action, int mods);
    static void errorCallback(int error, const char* description);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void moveCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xpos, double ypos);
    static void sizeCallback(GLFWwindow* window, int width, int height);
private:
    ///Animator mAnimator;                  //动画控制器 演示
    CameraEye mCamera;                      //相机
    TrackBallInteractor mInteractor;        //计算TrackBall点模块
    RendererGL mRenderer;                   //GL绘制
    GLFWwindow* mWindow;                    //窗口界面
};

#endif




