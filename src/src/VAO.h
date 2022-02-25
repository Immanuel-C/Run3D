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

	uint32_t operator!() {
		return m_ID;
	}

	bool operator==(int other) {
		return m_ID == other;
	}
private:
	uint32_t m_ID = 0;
};

