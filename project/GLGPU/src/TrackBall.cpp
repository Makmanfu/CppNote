
#include "stdafx.h"
#include "TrackBall.h"
#include <math.h>
#include <iostream>
#include <string.h>                         // memcpy
#include <glm/gtc/matrix_transform.hpp>     // lookAt// perspective
#include <glm/gtc/type_ptr.hpp>             // value_ptr
#include <glm/gtx/norm.hpp>                 // length2
#include <glm/vec3.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>            // pi
#include <glm/glm.hpp>

CameraEye::CameraEye()
{
    reset();
}

CameraEye::~CameraEye()
{
}

const glm::vec3& CameraEye::getCenter(void)
{
    return mCenter;
}

const glm::vec3& CameraEye::getEye(void)
{
    return mEye;
}

const glm::mat4& CameraEye::getMatrix(void)
{
    return mMatrix;
}

const float* CameraEye::getMatrixFlat(void)
{
    return glm::value_ptr(mMatrix);
}

const glm::vec3& CameraEye::getUp(void)
{
    return mUp;
}

void CameraEye::reset(void)
{
    mEye.x = 0.f;
    mEye.y = 0.f;
    mEye.z = 1.f;
    mCenter.x = 0.f;
    mCenter.y = 0.f;
    mCenter.z = 0.f;
    mUp.x = 0.f;
    mUp.y = 1.f;
    mUp.z = 0.f;
    update();
}

void CameraEye::setEye(float x, float y, float z)
{
    mEye.x = x;
    mEye.y = y;
    mEye.z = z;
}

void CameraEye::setEye(const glm::vec3& e)
{
    mEye = e;
}

void CameraEye::setCenter(float x, float y, float z)
{
    mCenter.x = x;
    mCenter.y = y;
    mCenter.z = z;
}

void CameraEye::setCenter(const glm::vec3& c)
{
    mCenter = c;
}

void CameraEye::setUp(float x, float y, float z)
{
    mUp.x = x;
    mUp.y = y;
    mUp.z = z;
}

void CameraEye::setUp(const glm::vec3& u)
{
    mUp = u;
}

void CameraEye::update(void)
{
    //lookAt 移动照相机（使用试图变换）来观察这个立方体
    mMatrix = glm::lookAt(mEye, mCenter, mUp);
}


RendererGL::RendererGL()
{
}

RendererGL::~RendererGL()
{
}

void RendererGL::init(void)
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    // Enable transparency.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void RendererGL::renderCenter(void)
{
    glPointSize(4);
    glBegin(GL_POINTS);
    if (mCamera)
    {
        const glm::vec3& p = mCamera->getCenter();
        glColor3f(1.f, 1.f, 0.f);
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

void RendererGL::renderAxes(void)
{
    glLineWidth(3);
    glBegin(GL_LINES);
    glColor3f(1.f, 0.f, 0.f);       //X轴
    glVertex3f(0.0f, 0.f, 0.f);
    glVertex3f(1.0f, 0.f, 0.f);
    glColor3f(0.f, 1.f, 0.f);       //Y轴
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.f, 1.0f, 0.f);
    glColor3f(0.f, 0.f, 1.f);       //Z轴
    glVertex3f(0.0f, 0.f, 0.f);
    glVertex3f(0.f, 0.f, 1.0f);
    glEnd();
}

void RendererGL::renderCube(void)
{
    GLfloat alpha = .9f;
    glBegin(GL_TRIANGLES);
    glColor4f(.8f, 0.4f, 0.4f, alpha);
    glVertex3f(0.2f, 0.f, 0.f);
    glVertex3f(0.2f, 0.2f, 0.f);
    glVertex3f(0.2f, 0.2f, 0.2f);
    glVertex3f(0.2f, 0.f, 0.f);
    glVertex3f(0.2f, 0.f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);
    glColor4f(.4f, 0.2f, 0.2f, alpha);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.f, 0.2f, 0.f);
    glVertex3f(0.f, 0.2f, 0.2f);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.f, 0.f, 0.2f);
    glVertex3f(0.f, 0.2f, 0.2f);
    glColor4f(0.4f, 0.8f, 0.4f, alpha);
    glVertex3f(0.f, 0.2f, 0.f);
    glVertex3f(0.2f, 0.2f, 0.f);
    glVertex3f(0.2f, 0.2f, 0.2f);
    glVertex3f(0.f, 0.2f, 0.f);
    glVertex3f(0.f, 0.2f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);
    glColor4f(0.2f, 0.4f, 0.2f, alpha);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.2f, 0.f, 0.f);
    glVertex3f(0.2f, 0.f, 0.2f);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.f, 0.f, 0.2f);
    glVertex3f(0.2f, 0.f, 0.2f);
    glColor4f(.4f, .4f, .8f, alpha);
    glVertex3f(0.f, 0.f, 0.2f);
    glVertex3f(0.f, 0.2f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);
    glVertex3f(0.f, 0.f, 0.2f);
    glVertex3f(0.2f, 0.f, 0.2f);
    glVertex3f(0.2f, 0.2f, 0.2f);
    glColor4f(0.2f, 0.2f, .4f, alpha);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.f, 0.2f, 0.f);
    glVertex3f(0.2f, 0.2f, 0.f);
    glVertex3f(0.f, 0.f, 0.f);
    glVertex3f(0.2f, 0.f, 0.f);
    glVertex3f(0.2f, 0.2f, 0.f);
    glEnd();
}

void RendererGL::renderCubeCenter(void)
{
    GLfloat alpha = .9f;
    glBegin(GL_TRIANGLES);
    glColor4f(.8f, 0.4f, 0.4f, alpha);
    glVertex3f(0.1f, -0.1f, -0.1f);
    glVertex3f(0.1f, 0.1f, -0.1f);
    glVertex3f(0.1f, 0.1f, 0.1f);
    glVertex3f(0.1f, -0.1f, -0.1f);
    glVertex3f(0.1f, -0.1f, 0.1f);
    glVertex3f(0.1f, 0.1f, 0.1f);
    glColor4f(.4f, 0.2f, 0.2f, alpha);
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(-0.1f, 0.1f, -0.1f);
    glVertex3f(-0.1f, 0.1f, 0.1f);
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(-0.1f, -0.1f, 0.1f);
    glVertex3f(-0.1f, 0.1f, 0.1f);
    glColor4f(0.4f, 0.8f, 0.4f, alpha);
    glVertex3f(-0.1f, 0.1f, -0.1f);
    glVertex3f(0.1f, 0.1f, -0.1f);
    glVertex3f(0.1f, 0.1f, 0.1f);
    glVertex3f(-0.1f, 0.1f, -0.1f);
    glVertex3f(-0.1f, 0.1f, 0.1f);
    glVertex3f(0.1f, 0.1f, 0.1f);
    glColor4f(0.2f, 0.4f, 0.2f, alpha);
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(0.1f, -0.1f, -0.1f);
    glVertex3f(0.1f, -0.1f, 0.1f);
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(-0.1f, -0.1f, 0.1f);
    glVertex3f(0.1f, -0.1f, 0.1f);
    glColor4f(.4f, .4f, .8f, alpha);
    glVertex3f(-0.1f, -0.1f, 0.1f);
    glVertex3f(-0.1f, 0.1f, 0.1f);
    glVertex3f(0.1f, 0.1f, 0.1f);
    glVertex3f(-0.1f, -0.1f, 0.1f);
    glVertex3f(0.1f, -0.1f, 0.1f);
    glVertex3f(0.1f, 0.1f, 0.1f);
    glColor4f(0.2f, 0.2f, .4f, alpha);
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(-0.1f, 0.1f, -0.1f);
    glVertex3f(0.1f, 0.1f, -0.1f);
    glVertex3f(-0.1f, -0.1f, -0.1f);
    glVertex3f(0.1f, -0.1f, -0.1f);
    glVertex3f(0.1f, 0.1f, -0.1f);
    glEnd();
}

void RendererGL::renderGEO(void)
{
    //没有写哦
}

void RendererGL::renderGrid(void)
{
    float p, d = .1, mn = -1.f, mx = 1.f, eps = -1e-4;
    int i, n = 20;
#define WHITE() glColor4f(1.f, 1.f, 1.f, .2f)
    glLineWidth(1);
    glBegin(GL_LINES);
    for (i = 0; i <= n; ++i)
    {
        p = mn + i * d;
        if (i == 0 || i == 10 || i == n)
            glColor4f(0.f, 1.f, 0.f, .3f);
        else
            WHITE();
        glVertex3f(p, mn, eps);
        glVertex3f(p, mx, eps);
        if (i == 0 || i == 10 || i == n)
            glColor4f(1.f, 0.f, 0.f, .3f);
        else
            WHITE();
        glVertex3f(mn, p, eps);
        glVertex3f(mx, p, eps);
    }
    glEnd();
#undef WHITE
}

void RendererGL::renderClear(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0f);
}

void RendererGL::renderObjects(void)
{
    renderCenter();
    renderGrid();
    renderAxes();
    //renderCube();
    renderCubeCenter();     //绘制中心正方体
}

void RendererGL::render(void)
{
    renderClear();
    glPushMatrix();
    glMultMatrixf(glm::value_ptr(mProjection));
    if (mCamera)
    {
        glPushMatrix();
        glMultMatrixf(mCamera->getMatrixFlat());
    }
    //++++++++++++++++++++++++++++++
    //++++++++++++++++++++++++++++++
    //绘制图像
    renderObjects();
    if (mCamera)
        glPopMatrix();
    glPopMatrix();
}

void RendererGL::resize(int width, int height)
{
    if (0 == height)
    {
        height = 1; // Avoid division by zero.
    }
    GLfloat ratio = width / (GLfloat)height;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    mProjection = glm::perspective(glm::radians(45.0f), ratio, .01f, 100.f);
}

const CameraEye* RendererGL::getCamera(void)
{
    return mCamera;
}

void RendererGL::setCamera(CameraEye* c)
{
    mCamera = c;
}


const glm::vec3 TrackBallInteractor::X(1.f, 0.f, 0.f);
const glm::vec3 TrackBallInteractor::Y(0.f, 1.f, 0.f);
const glm::vec3 TrackBallInteractor::Z(0.f, 0.f, 1.f);

TrackBallInteractor::TrackBallInteractor() :
    mCameraMotionLeftClick(ARC),
    mCameraMotionMiddleClick(ROLL),
    mCameraMotionRightClick(FIRSTPERSON),
    mCameraMotionScroll(ZOOM),
    mHeight(1),
    mIsDragging(false),
    mIsLeftClick(false),
    mIsMiddleClick(false),
    mIsRightClick(false),
    mIsScrolling(false),
    mPanScale(.005f),
    mRollScale(.005f),
    mRollSum(0.f),
    mRotation(1.f, 0, 0, 0),
    mRotationSum(1.f, 0, 0, 0),
    mSpeed(1.f),
    mWidth(1),
    mZoomScale(.1f),
    mZoomSum(0.f)
{
}

TrackBallInteractor::~TrackBallInteractor()
{
}

char TrackBallInteractor::clickQuadrant(float x, float y)
{
    float halfw = .5 * mWidth;
    float halfh = .5 * mHeight;
    if (x > halfw)
    {
        // Opengl image coordinates origin is upperleft.
        if (y < halfh)
            return 1;
        else
            return 4;
    }
    else
    {
        if (y < halfh)
            return 2;
        else
            return 3;
    }
}

void TrackBallInteractor::computeCameraEye(glm::vec3& eye)
{
    glm::vec3 orientation = mRotationSum * Z;
    if (mZoomSum)
    {
        mTranslateLength += mZoomScale * mZoomSum;
        mZoomSum = 0; // Freeze zooming after applying.
    }
    eye = mTranslateLength * orientation + mCamera->getCenter();
}

void TrackBallInteractor::computeCameraUp(glm::vec3& up)
{
    up = glm::normalize(mRotationSum * Y);
}

void TrackBallInteractor::computePan(glm::vec3& pan)
{
    glm::vec2 click = mClickPoint - mPrevClickPoint;
    glm::vec3 look = mCamera->getEye() - mCamera->getCenter();
    float length = glm::length(look);
    glm::vec3 right = glm::normalize(mRotationSum * X);
    pan = (mCamera->getUp() * -click.y + right * click.x) *
          mPanScale * mSpeed * length;
}

void TrackBallInteractor::computePointOnSphere(const glm::vec2& point, glm::vec3& result)
{
    // https://www.opengl.org/wiki/Object_Mouse_Trackball
    float x = (2.f * point.x - mWidth) / mWidth;
    float y = (mHeight - 2.f * point.y) / mHeight;
    float length2 = x * x + y * y;
    if (length2 <= .5)
        result.z = sqrt(1.0 - length2);
    else
        result.z = 0.5 / sqrt(length2);
    float norm = 1.0 / sqrt(length2 + result.z * result.z);
    result.x = x * norm;
    result.y = y * norm;
    result.z *= norm;
}

void TrackBallInteractor::computeRotationBetweenVectors(const glm::vec3& u, const glm::vec3& v, glm::quat& result)
{
    float cosTheta = glm::dot(u, v);
    glm::vec3 rotationAxis(glm::uninitialize);
    static const float EPSILON = 1.0e-5f;
    if (cosTheta < -1.0f + EPSILON)
    {
        // Parallel and opposite directions.
        rotationAxis = glm::cross(glm::vec3(0.f, 0.f, 1.f), u);
        if (glm::length2(rotationAxis) < 0.01)
        {
            // Still parallel, retry.
            rotationAxis = glm::cross(glm::vec3(1.f, 0.f, 0.f), u);
        }
        rotationAxis = glm::normalize(rotationAxis);
        result = glm::angleAxis(180.0f, rotationAxis);
    }
    else if (cosTheta > 1.0f - EPSILON)
    {
        // Parallel and same direction.
        result = glm::quat(1, 0, 0, 0);
        return;
    }
    else
    {
        float theta = acos(cosTheta);
        rotationAxis = glm::cross(u, v);
        rotationAxis = glm::normalize(rotationAxis);
        result = glm::angleAxis(theta * mSpeed, rotationAxis);
    }
}

void TrackBallInteractor::drag()
{
    if (mPrevClickPoint == mClickPoint)
    {
        // Not moving during drag state, so skip unnecessary processing.
        return;
    }
    computePointOnSphere(mClickPoint, mStopVector);
    computeRotationBetweenVectors(mStartVector,
                                  mStopVector,
                                  mRotation);
    // Reverse so scene moves with cursor and not away due to camera model.
    mRotation = glm::inverse(mRotation);
    drag(mIsLeftClick, mCameraMotionLeftClick);
    drag(mIsMiddleClick, mCameraMotionMiddleClick);
    drag(mIsRightClick, mCameraMotionRightClick);
    // After applying drag, reset relative start state.
    mPrevClickPoint = mClickPoint;
    mStartVector = mStopVector;
}

void TrackBallInteractor::drag(bool isClicked, CameraMotionType motion)
{
    if (!isClicked)
        return;
    switch (motion)
    {
        case ARC:
            dragArc();
            break;
        case FIRSTPERSON:
            dragFirstPerson();
            break;
        case PAN:
            dragPan();
            break;
        case ROLL:
            rollCamera();
            break;
        case ZOOM:
            dragZoom();
            break;
        default:
            break;
    }
}

void TrackBallInteractor::dragArc()
{
    mRotationSum *= mRotation; // Accumulate quaternions.
    updateCameraEyeUp(true, true);
}

void TrackBallInteractor::dragFirstPerson()
{
    glm::vec3 pan(glm::uninitialize);
    computePan(pan);
    mCamera->setCenter(pan + mCamera->getCenter());
    mCamera->update();
    freezeTransform();
}

void TrackBallInteractor::dragPan()
{
    glm::vec3 pan(glm::uninitialize);
    computePan(pan);
    mCamera->setCenter(pan + mCamera->getCenter());
    mCamera->setEye(pan + mCamera->getEye());
    mCamera->update();
    freezeTransform();
}

void TrackBallInteractor::dragZoom()
{
    glm::vec2 dir = mClickPoint - mPrevClickPoint;
    float ax = fabs(dir.x);
    float ay = fabs(dir.y);
    if (ay >= ax)
        setScrollDirection(dir.y <= 0);
    else
        setScrollDirection(dir.x <= 0);
    updateCameraEyeUp(true, false);
}

void TrackBallInteractor::freezeTransform()
{
    if (mCamera)
    {
        // Opengl is ZYX order.
        // Flip orientation to rotate scene with sticky cursor.
        mRotationSum = glm::inverse(glm::quat(mCamera->getMatrix()));
        mTranslateLength = glm::length(mCamera->getEye() - mCamera->getCenter());
    }
}

CameraEye* TrackBallInteractor::getCamera(void)
{
    return mCamera;
}

TrackBallInteractor::CameraMotionType TrackBallInteractor::getMotionLeftClick(void)
{
    return mCameraMotionLeftClick;
}

TrackBallInteractor::CameraMotionType TrackBallInteractor::getMotionMiddleClick(void)
{
    return mCameraMotionMiddleClick;
}

TrackBallInteractor::CameraMotionType TrackBallInteractor::getMotionRightClick(void)
{
    return mCameraMotionRightClick;
}

TrackBallInteractor::CameraMotionType TrackBallInteractor::getMotionScroll(void)
{
    return mCameraMotionScroll;
}

void TrackBallInteractor::rollCamera()
{
    glm::vec2 delta = mClickPoint - mPrevClickPoint;
    char quad = clickQuadrant(mClickPoint.x, mClickPoint.y);
    //判断每个象限
    switch (quad)
    {
        case 1:
            delta.y = -delta.y;
            delta.x = -delta.x;
            break;
        case 2:
            delta.x = -delta.x;
            break;
        case 3:
            break;
        case 4:
            delta.y = -delta.y;
        default:
            break;
    }
    glm::vec3 axis = glm::normalize(mCamera->getCenter() - mCamera->getEye());
    float angle = mRollScale * mSpeed * (delta.x + delta.y + mRollSum);
    glm::quat rot = glm::angleAxis(angle, axis);
    mCamera->setUp(rot * mCamera->getUp());
    mCamera->update();
    freezeTransform();
    mRollSum = 0;
}

void TrackBallInteractor::scroll()
{
    switch (mCameraMotionScroll)
    {
        case ROLL:
            rollCamera();
            break;
        case ZOOM:
            updateCameraEyeUp(true, false);
            break;
        default:
            break;
    }
}

void TrackBallInteractor::setCamera(CameraEye* c)
{
    mCamera = c;
    freezeTransform();
}

void TrackBallInteractor::setClickPoint(double x, double y)
{
    mPrevClickPoint = mClickPoint;
    mClickPoint.x = x;
    mClickPoint.y = y;
}

void TrackBallInteractor::setLeftClicked(bool value)
{
    mIsLeftClick = value;
}

void TrackBallInteractor::setMiddleClicked(bool value)
{
    mIsMiddleClick = value;
}

void TrackBallInteractor::setMotionLeftClick(CameraMotionType motion)
{
    mCameraMotionLeftClick = motion;
}

void TrackBallInteractor::setMotionMiddleClick(CameraMotionType motion)
{
    mCameraMotionMiddleClick = motion;
}

void TrackBallInteractor::setMotionRightClick(CameraMotionType motion)
{
    mCameraMotionRightClick = motion;
}

void TrackBallInteractor::setMotionScroll(CameraMotionType motion)
{
    mCameraMotionScroll = motion;
}

void TrackBallInteractor::setRightClicked(bool value)
{
    mIsRightClick = value;
}

void TrackBallInteractor::setScreenSize(float width, float height)
{
    if (width > 1 && height > 1)
    {
        mWidth = width;
        mHeight = height;
    }
}

void TrackBallInteractor::setScrollDirection(bool up)
{
    mIsScrolling = true;
    float inc = mSpeed * (up ? -1.f : 1.f);
    mZoomSum += inc;
    mRollSum += inc;
}

void TrackBallInteractor::setSpeed(float s)
{
    mSpeed = s;
}

void TrackBallInteractor::update()
{
    const bool isClick = mIsLeftClick || mIsMiddleClick || mIsRightClick;
    if (!mIsDragging)
    {
        if (isClick)
        {
            mIsDragging = true;
            computePointOnSphere(mClickPoint, mStartVector);
        }
        else if (mIsScrolling)
        {
            scroll();
            mIsScrolling = false;
        }
    }
    else
    {
        if (isClick)
            drag();
        else
            mIsDragging = false;
    }
}

void TrackBallInteractor::updateCameraEyeUp(bool eye, bool up)
{
    if (eye)
    {
        glm::vec3 eye(glm::uninitialize);
        computeCameraEye(eye);
        mCamera->setEye(eye);
    }
    if (up)
    {
        glm::vec3 up(glm::uninitialize);
        computeCameraUp(up);
        mCamera->setUp(up);
    }
    mCamera->update();
}



#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

int TrackBall_Test(int argc, char** argv)
{
    return RenderWinGLFW::instance().run(600, 600);
}

static const char* HELP =
"           c: Print camera Eye, Center, Up\n"
"           r: Reset view\n"
"           t: Toggle right button to do Pan or First-Person\n"
"     x, y, z: Snap camera to axis\n"
"   Hold Ctrl: Increase speed\n"
"  Hold Shift: Reduce speed\n"
"  Left-Click: Rotate\n"
"Middle-Click: Pan or First-Person\n"
" Right-Click: Roll\n"
"Scroll-Wheel: Dolly (zoom)\n";

RenderWinGLFW::RenderWinGLFW() :
mWindow(0)
{
}

RenderWinGLFW::~RenderWinGLFW()
{
}

void RenderWinGLFW::buttonCallback(GLFWwindow* window, int button, int action, int mods)
{
    switch (action)
    {
    case GLFW_PRESS:                            //鼠标按下
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:        //鼠标左键
            instance().mInteractor.setLeftClicked(true);
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:      //鼠标中键
            instance().mInteractor.setMiddleClicked(true);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:       //鼠标右键
            instance().mInteractor.setRightClicked(true);
            break;
        }
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        instance().mInteractor.setClickPoint(xpos, ypos);
        break;
    }
    case GLFW_RELEASE:
    {
        switch (button)
        {
        case GLFW_MOUSE_BUTTON_LEFT:
            instance().mInteractor.setLeftClicked(false);
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            instance().mInteractor.setMiddleClicked(false);
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            instance().mInteractor.setRightClicked(false);
            break;
        }
        break;
    }
    default:
        break;
    }
}

void RenderWinGLFW::errorCallback(int error, const char* description)
{
    std::cerr << description << std::endl;
}

RenderWinGLFW& RenderWinGLFW::instance()
{
    static RenderWinGLFW i;
    return i;
}

void RenderWinGLFW::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    float length;
    switch (action)
    {
    case GLFW_PRESS:
        switch (key)
        {
        case GLFW_KEY_ESCAPE:
            // Exit app on ESC key.
            glfwSetWindowShouldClose(window, GL_TRUE);
            break;
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
            instance().mInteractor.setSpeed(5.f);
            break;
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            instance().mInteractor.setSpeed(.1f);
            break;
        case GLFW_KEY_F1:   //旋转动画
            ///instance().mAnimator.setAnimation(Animator::ORBIT);
            break;
        case GLFW_KEY_C:    //打印点
            std::cout
                << "(" << instance().mCamera.getEye().x
                << "," << instance().mCamera.getEye().y
                << "," << instance().mCamera.getEye().z << ") "
                << "(" << instance().mCamera.getCenter().x
                << "," << instance().mCamera.getCenter().y
                << "," << instance().mCamera.getCenter().z << ") "
                << "(" << instance().mCamera.getUp().x
                << "," << instance().mCamera.getUp().y
                << "," << instance().mCamera.getUp().z << ")\n";
            break;
        case GLFW_KEY_R:
            // Reset the view.
            instance().mCamera.reset();
            instance().mInteractor.setCamera(&instance().mCamera);
            break;
        case GLFW_KEY_T:
            // Toogle motion type.
            if (instance().mInteractor.getMotionRightClick() ==
                TrackBallInteractor::FIRSTPERSON)
            {
                instance().mInteractor.setMotionRightClick(
                    TrackBallInteractor::PAN);
            }
            else
            {
                instance().mInteractor.setMotionRightClick(
                    TrackBallInteractor::FIRSTPERSON);
            }
            break;
        case GLFW_KEY_X:
            // Snap view to axis.
            length = glm::length(instance().mCamera.getEye() -
                instance().mCamera.getCenter());
            instance().mCamera.setEye(length, 0, 0);
            instance().mCamera.setUp(0, 1, 0);
            instance().mCamera.update();
            instance().mInteractor.setCamera(&instance().mCamera);
            break;
        case GLFW_KEY_Y:
            length = glm::length(instance().mCamera.getEye() -
                instance().mCamera.getCenter());
            instance().mCamera.setEye(0, length, 0);
            instance().mCamera.setUp(1, 0, 0);
            instance().mCamera.update();
            instance().mInteractor.setCamera(&instance().mCamera);
            break;
        case GLFW_KEY_Z:
            length = glm::length(instance().mCamera.getEye() -
                instance().mCamera.getCenter());
            instance().mCamera.setEye(0, 0, length);
            instance().mCamera.setUp(1, 0, 0);
            instance().mCamera.update();
            instance().mInteractor.setCamera(&instance().mCamera);
            break;
        default:
            break;
        }
        break;
    case GLFW_RELEASE:
        switch (key)
        {
        case GLFW_KEY_LEFT_CONTROL:
        case GLFW_KEY_RIGHT_CONTROL:
        case GLFW_KEY_LEFT_SHIFT:
        case GLFW_KEY_RIGHT_SHIFT:
            instance().mInteractor.setSpeed(1.f);
            break;
        }
        break;
    default:
        break;
    }
}

void RenderWinGLFW::moveCallback(GLFWwindow* window, double xpos, double ypos)
{
    instance().mInteractor.setClickPoint(xpos, ypos);
}

void RenderWinGLFW::scrollCallback(GLFWwindow* window, double xpos, double ypos)
{
    instance().mInteractor.setScrollDirection(xpos + ypos > 0 ? true : false);
}

void RenderWinGLFW::sizeCallback(GLFWwindow* window, int width, int height)
{
    instance().mRenderer.resize(width, height);
    instance().mInteractor.setScreenSize(width, height);
    ///instance().mAnimator.setScreenSize(width, height);
}

int RenderWinGLFW::run(int width, int height)
{
    int exitcode = EXIT_SUCCESS;
    //错误返回回调
    glfwSetErrorCallback(&RenderWinGLFW::errorCallback);
    //初始化GL
    if (!glfwInit())
    {
        exitcode = EXIT_FAILURE;
        goto recover;
    }
    mWindow = glfwCreateWindow(width, height, "3DVIEW", //"TrackBall GLFW Example"
        NULL, NULL);
    if (!mWindow)
    {
        glfwTerminate();
        exitcode = EXIT_FAILURE;
        goto recover;
    }
    std::cout << HELP;
    //mCamera.setUp(1.f, 1.f, 1.f);
    glfwMakeContextCurrent(mWindow);
    glfwSwapInterval(1);
    glfwSetCursorPosCallback(mWindow, &RenderWinGLFW::moveCallback);
    glfwSetKeyCallback(mWindow, &RenderWinGLFW::keyCallback);
    glfwSetMouseButtonCallback(mWindow, &RenderWinGLFW::buttonCallback);
    glfwSetScrollCallback(mWindow, &RenderWinGLFW::scrollCallback);
    glfwSetWindowSizeCallback(mWindow, &RenderWinGLFW::sizeCallback);
    mInteractor.setCamera(&mCamera);
    mRenderer.setCamera(&mCamera);
    ///mAnimator.setInteractor(& mInteractor);
    mRenderer.init();
    sizeCallback(mWindow, width, height); // Set initial size.
    while (!glfwWindowShouldClose(mWindow))
    {
        ///mAnimator.animate();
        mInteractor.update();
        //GL的绘制
        mRenderer.render();
        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }
    glfwDestroyWindow(mWindow);
    glfwTerminate();
recover:
    return exitcode;
}




