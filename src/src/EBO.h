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
    EBO(std::array<uint32_t, _size> indices) {
        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);
    }

    EBO(std::vector<uint32_t> indices) {
        glGenBuffers(1, &m_ID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(uint32_t), indices.data(), GL_DYNAMIC_DRAW);
    }
    
    EBO(uint32_t* indices, size_t lenOfindices);

    EBO() {}

    uint32_t getID();
    void bind();
    void unbind();
    void destroy();
private:
    uint32_t m_ID = 0;
};