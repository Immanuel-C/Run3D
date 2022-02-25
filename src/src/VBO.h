#pragma once


#include <iostream>
#include <glad/glad.h>

#include <vector>

class VBO
{
public:
	template <size_t _size>
    VBO(std::array<float, _size> vertices);
    VBO(std::vector<float> vertices);

    VBO() {}

	uint32_t getID();
	void bind();
    void unbind();
    void destroy();
private:
	uint32_t m_ID = 0;
};

template<size_t _size>
inline VBO::VBO(std::array<float, _size> vertices)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * _size, vertices.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(3 * sizeof(float)));
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (const void*)(6 * sizeof(float)));

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
}

