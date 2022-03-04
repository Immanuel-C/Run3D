#pragma once

#include <iostream>
#include <glad/glad.h>

class VAO
{
public:
	VAO();

	uint32_t getID();
	void bind();
	void unbind();

	void destroy();

private:
	uint32_t m_ID = 0;
};

