#pragma once


#pragma once


#include <glad/glad.h>
#include <cstdint>
#include <array>
#include <vector>

class EBO
{
public:
    template <size_t _size>
    EBO(std::array<uint8_t, _size> indices);
    EBO(std::vector<uint8_t> indices);

    EBO() {}

    uint32_t getID();
    void bind();
    void unbind();
    void destroy();
private:
    uint32_t m_ID = 0;
};

template<size_t _size>
inline EBO::EBO(std::array<uint8_t, _size> indices)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * _size, indices.data(), GL_DYNAMIC_DRAW);
}
