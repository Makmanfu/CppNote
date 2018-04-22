#ifndef RSMZ_RENDERWINDOWGLFW_H
#define RSMZ_RENDERWINDOWGLFW_H

#include <GLFW/glfw3.h>
//#include "TrackBall.h"
#include "TrackBallInteractor.h"
#include "RendererOpenGL1.h"
#include "Animator.h"

namespace rsmz
{
    /**
        Example using GLFW.
        Only one window instance is supported, but this could be extended by
        using a registry class to scale the static callback handlers.
    */
    class RenderWindowGLFW
    {
    public:
        RenderWindowGLFW();
        ~RenderWindowGLFW();
    public:
        //����
        static RenderWindowGLFW& instance();
        int run(int width, int height);
        //�ص�����
        static void buttonCallback(GLFWwindow* window, int button, int action, int mods);
        static void errorCallback(int error, const char* description);
        static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
        static void moveCallback(GLFWwindow* window, double xpos, double ypos);
        static void scrollCallback(GLFWwindow* window, double xpos, double ypos);
        static void sizeCallback(GLFWwindow* window, int width, int height);
    private:
        Animator mAnimator;                     //���������� ��ʾ
        Camera mCamera;                         //���
        TrackBallInteractor mInteractor;        //����TrackBall��ģ��
        RendererOpenGL1 mRenderer;              //GL����
        GLFWwindow* mWindow;                    //���ڽ���
    };
}

#endif



