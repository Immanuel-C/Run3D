#include "VAO.h"

#include "Window.h"

VAO::VAO()
{
	Window::get();

	glGenVertexArrays(1, &m_ID);
}

uint32_t VAO::getID()
{
	return m_ID;
}

void VAO::bind()
{
	glBindVertexArray(m_ID);
}

void VAO::unbind()
{
	glBindVertexArray(0);
}

void VAO::destroy()
{
	glDeleteVertexArrays(1, &m_ID);
}
