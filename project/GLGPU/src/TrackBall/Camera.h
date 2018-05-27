#ifndef CAMERA_H
#define CAMERA_H

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>

namespace rsmz
{
    //�����(ת���Ӿ�����Ϳ������)
    class Camera
    {
    public:
        Camera();
        ~Camera();
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
}

#endif


