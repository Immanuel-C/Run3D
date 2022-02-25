#include "Camera.h"


void windowResizeCallback(GLFWwindow* window, int width, int height)
{
    Window::get()->setSize(width, height);

    Camera3D* camera = (Camera3D*)glfwGetWindowUserPointer(window);

    camera->m_aspectRatio = (float)width / (float)height;
    camera->m_projectionMat = glm::perspective(camera->m_fov, camera->m_aspectRatio, camera->m_nearZ, camera->m_farZ);

    glViewport(0, 0, width, height);
}

Camera3D::Camera3D(float fov, float aspectRatio, float nearZ, float farZ)
    : m_projectionMat{ glm::perspective(fov, aspectRatio, nearZ, farZ) },
      m_aspectRatio{ aspectRatio },
      m_fov{ fov },
      m_farZ{ farZ },
      m_nearZ{ nearZ }
{
    glfwSetWindowSizeCallback(Window::get()->getGLFWwindow(), windowResizeCallback);
    glfwSetWindowUserPointer(Window::get()->getGLFWwindow(), this);
}

glm::mat4 Camera3D::getProjectionMatrix()
{
    return m_projectionMat;
}

void Camera3D::setProjectionMatrix(glm::mat4 newProjMat)
{
    m_projectionMat = newProjMat;
    recalculateMatrices();
}

glm::mat4 Camera3D::getViewMatrix()
{
    return m_viewMat;
}

void Camera3D::setViewMatrix(glm::mat4 newViewMat)
{
    m_viewMat = newViewMat;
    recalculateMatrices();
}


void Camera3D::setPosition(glm::vec3 position)
{
    m_position = position;
    recalculateMatrices();
}

glm::vec3 Camera3D::getPosition()
{
    return m_position;
}

void Camera3D::setUp(glm::vec3 up)
{
    m_up = up;
    recalculateMatrices();
}

glm::vec3 Camera3D::getUp()
{
    return m_up;
}

void Camera3D::setFront(glm::vec3 front)
{
    m_front = front;
    recalculateMatrices();
}

glm::vec3 Camera3D::getFront()
{
    return m_front;
}

void Camera3D::move(glm::vec3 velocity)
{
    m_position += velocity;
    recalculateMatrices();
}


void Camera3D::recalculateMatrices()
{
    m_viewMat = glm::lookAt(m_position, m_position + m_front, m_up);
}


