#pragma once
#include <glad/glad.h>

#include <string>
#include <array>
#include <vector>
#include <iostream>

#define R3D_USE 1
#define R3D_DONT_USE 0

class ITexture
{
public:
    inline uint32_t getID() { return m_ID; }

    virtual void bind() {}
    virtual void unbind() {}

protected:
    uint32_t m_ID = 0;
    int m_width = 0, m_height = 0, m_colourChannels = 0;
};

struct TextureEffects {
    struct {
        int repeat = 0, mirroredRepeat = 0, clampToEdge = 0, clampToBorder = 0;
        float borderColours[4] = {1.0f, 1.0f, 1.0f, 1.0f};  
    } wrapping;

    struct {
        int bilinear = 0, nearest = 0;
    } filtering;
};

class Texture : public ITexture {
public:
    Texture(const std::string& filePath, TextureEffects effects);

    Texture() {}

    inline void bind() override { glBindTexture(GL_TEXTURE_2D, m_ID); }
    inline void unbind() override { glBindTexture(GL_TEXTURE_2D, 0); }
};

class CubeMap : public ITexture {
public:
    CubeMap(std::string* filePaths, size_t lenOfFilePaths, TextureEffects effects);

    template <size_t _size>
    CubeMap(std::array<std::string, _size> filePaths, TextureEffects effects) {
        CubeMap(filePaths.data(), filePaths.size(), effects);
    }

    CubeMap(std::vector<std::string> filePaths, TextureEffects effects) {
        CubeMap(filePaths.data(), filePaths.size(), effects);
    }

    CubeMap() {}

    inline void bind() override { glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID); }
    inline void unbind() override { glBindTexture(GL_TEXTURE_CUBE_MAP, 0); }
};