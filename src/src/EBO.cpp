#include "EBO.h"

#include <iostream>

EBO::EBO(uint32_t* indices, size_t lenOfindices)
{
    glGenBuffers(1, &m_ID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, lenOfindices * sizeof(uint32_t), indices, GL_DYNAMIC_DRAW);
}

uint32_t EBO::getID()
{
    return m_ID;
}

void EBO::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
}

void EBO::unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::destroy()
{
    glDeleteBuffers(1, &m_ID);
}
