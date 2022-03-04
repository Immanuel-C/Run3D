#pragma once


#include <iostream>
#include <glad/glad.h>

#include <vector>

class VBO
{
public:
	template <size_t _size>
    VBO(std::array<float, _size> vertices) { VBO(vertices.data(), vertices.size()); }
    VBO(std::vector<float> vertices) { VBO(vertices.data(), vertices.size()); }

    VBO(float* vertices, size_t lenOfVertices);

    VBO() {}

	uint32_t getID();
	void bind();
    void unbind();
    void destroy();
private:
	uint32_t m_ID = 0;
};

