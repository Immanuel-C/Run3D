#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include "Utils.h"
#include "Input.h"


class Window
{
public:
	static Window* get();

	void destroy();

	inline uint32_t getWidth() const  { return m_windowWidth; }
	inline uint32_t getHeight() const { return m_windowHeight; }
	inline const std::string& getTitle() const { return m_windowTitle; }

	inline GLFWwindow* getGLFWwindow() { return m_window; }

	inline void setWidth(uint32_t newWidth) { setSize(newWidth, m_windowHeight); }
	inline void setHeight(uint32_t newHeight) { setSize(m_windowWidth, newHeight); }
	
	inline void setSize(uint32_t newWidth, uint32_t newHeight) {
		m_windowWidth = newWidth;  
		m_windowHeight = newHeight; 
		glfwSetWindowSize(m_window, m_windowWidth, m_windowHeight); 
		glViewport(0, 0, m_windowWidth, m_windowHeight);
	}

	inline void setTitle(const std::string& title) { get()->m_windowTitle = title; glfwSetWindowTitle(m_window, title.c_str()); }

	operator GLFWwindow*() const { return m_window; }

	void doBackEndStuff();

	inline bool shouldClose() { return glfwWindowShouldClose(m_window); }

	inline void useVsync(bool useVsync) { useVsync == true ? glfwSwapInterval(1) : glfwSwapInterval(0); }

	inline void setColor(float r, float g, float b, float a) { glClearColor(r, g, b, a); }
	inline void clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }


private:
	Window();

	static void windowResizeCallback(GLFWwindow* window, int width, int height);

	static Window* m_instance;

	GLFWwindow* m_window;

	uint32_t m_windowWidth = 1;
	uint32_t m_windowHeight = 1;
	std::string m_windowTitle = "";
};

