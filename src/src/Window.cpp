#include "Window.h"

void GLAPIENTRY
MessageCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	std::cout << message << "\n\n";
}

Window* Window::m_instance;

Window* Window::get() 
{
	if (!m_instance) {
		m_instance = new Window;
	}

	return m_instance;
}

void Window::destroy()
{
	glfwDestroyWindow(m_window);
	glfwTerminate();

	delete m_instance;
}

void Window::doBackEndStuff()
{
	glfwPollEvents();
	glfwSwapBuffers(m_window);
}

Window::Window()
{
	if (!glfwInit()) {
		std::cerr << "Error: Failed to init GLFW!";
	}

	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, m_windowTitle.c_str(), nullptr, nullptr);

	glfwMakeContextCurrent(m_window);

	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize GLAD";
	}

	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(MessageCallback, nullptr);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	glViewport(0, 0, m_windowWidth, m_windowHeight);


	// Extension: GL_ARB_parallel_shader_compile
	// Request 4 threads for compiling shaders
	glMaxShaderCompilerThreadsARB(4);
}

