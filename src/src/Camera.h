#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.h"
#include <GLFW/glfw3.h>

class Camera3D
{
public:
	Camera3D(float fov, float aspectRatio, float nearZ, float farZ);

	Camera3D() {}

	glm::mat4 getProjectionMatrix();
	void setProjectionMatrix(glm::mat4 newProjMat);
	glm::mat4 getViewMatrix();
	void setViewMatrix(glm::mat4 newViewMat);

	void setPosition(glm::vec3 position);
	glm::vec3 getPosition();
	void setUp(glm::vec3 up);
	glm::vec3 getUp();
	void setFront(glm::vec3 front);
	glm::vec3 getFront();

	void move(glm::vec3 velocity);
private:
	void recalculateMatrices();
	
	friend void windowResizeCallback(GLFWwindow* window, int width, int height);

	float m_fov = 0.0f, m_aspectRatio = 0.0f, m_nearZ = 0.0f, m_farZ = 0.0f;

	glm::mat4 m_projectionMat{ 1.0f };

	glm::vec3 m_position{ 0.0f };
	glm::vec3 m_up = glm::vec3{ 0.0f, 1.0f, 0.0f };
	glm::vec3 m_front = glm::vec3{0.0f, 0.0f, 0.0f};
	glm::mat4 m_viewMat{ 1.0f };
};

