#include "Texture.h"

#include "stb_image.h"

void setTextureWrappingParams(int type, int param) {
    glTexParameteri(type, GL_TEXTURE_WRAP_S, param);
    glTexParameteri(type, GL_TEXTURE_WRAP_T, param);
    glTexParameteri(type, GL_TEXTURE_WRAP_R, param);
}

Texture::Texture(const std::string& filePath, TextureEffects effects) {
    stbi_set_flip_vertically_on_load(true);

    uint8_t* imgBuffer = stbi_load
    (
        filePath.c_str(), 
        &m_width, 
        &m_height, 
        &m_colourChannels, 
        NULL
    );

    if (!imgBuffer) {
        std::cerr << "Failed to load image: " << filePath << '\n'; 
        stbi_image_free(imgBuffer);
        return;
    }

    glActiveTexture(GL_TEXTURE0);

    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_2D, m_ID);

    if (effects.wrapping.repeat) {
        setTextureWrappingParams(GL_TEXTURE_2D, GL_REPEAT);
    } else if (effects.wrapping.mirroredRepeat) {
        setTextureWrappingParams(GL_TEXTURE_2D, GL_MIRRORED_REPEAT);
    } else if (effects.wrapping.clampToEdge) {
        setTextureWrappingParams(GL_TEXTURE_2D, GL_CLAMP_TO_EDGE);
    } else if (effects.wrapping.clampToBorder) {
        setTextureWrappingParams(GL_TEXTURE_2D, GL_CLAMP_TO_BORDER);
        glGetTextureParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, effects.wrapping.borderColours);
    }

    if (effects.filtering.bilinear) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else if (effects.filtering.nearest) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    if (m_colourChannels == 4) {
        glTexImage2D
        (
            GL_TEXTURE_2D, 
            NULL, 
            GL_RGBA, 
            m_width, 
            m_height, 
            NULL, 
            GL_RGBA, 
            GL_UNSIGNED_BYTE, 
            imgBuffer
        );
    } else if (m_colourChannels == 3) {
        glTexImage2D
        (
            GL_TEXTURE_2D, 
            NULL, 
            GL_RGB, 
            m_width, 
            m_height, 
            NULL, 
            GL_RGB, 
            GL_UNSIGNED_BYTE, 
            imgBuffer
        );
    } else if (m_colourChannels == 2) {
        glTexImage2D
        (
            GL_TEXTURE_2D, 
            NULL, 
            GL_RG, 
            m_width, 
            m_height, 
            NULL, 
            GL_RG, 
            GL_UNSIGNED_BYTE, 
            imgBuffer
        );
    } else if (m_colourChannels == 1) {
        glTexImage2D
        (
            GL_TEXTURE_2D, 
            NULL, 
            GL_R, 
            m_width, 
            m_height, 
            NULL, 
            GL_R, 
            GL_UNSIGNED_BYTE, 
            imgBuffer
        );
    } else {
        std::cerr << "Could not get colour channels from image: " << filePath;
        stbi_image_free(imgBuffer);
        return;
    }

    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(imgBuffer);

    stbi_set_flip_vertically_on_load(false);
}

CubeMap::CubeMap(std::string* filePaths, size_t lenOfFilePaths, TextureEffects effects) {
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_ID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_ID);

    for (uint32_t i = 0; i < lenOfFilePaths; i++) {
        uint8_t* imgBuffer = stbi_load
        (
            filePaths[i].c_str(), 
            &m_width, 
            &m_height, 
            &m_colourChannels, 
            NULL
        );

        if (!imgBuffer) {
            std::cerr << "Failed to load image: " << filePaths[i] << '\n'; 
            stbi_image_free(imgBuffer);
        }

        if (m_colourChannels == 4) {
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                NULL, 
                GL_RGBA, 
                m_width, 
                m_height, 
                NULL, 
                GL_RGBA, 
                GL_UNSIGNED_BYTE, 
                imgBuffer
            );
        } else if (m_colourChannels == 3) {
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                NULL, 
                GL_RGB, 
                m_width, 
                m_height, 
                NULL, 
                GL_RGB, 
                GL_UNSIGNED_BYTE, 
                imgBuffer
            );
        } else if (m_colourChannels == 2) {
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                NULL, 
                GL_RG, 
                m_width, 
                m_height, 
                NULL, 
                GL_RG, 
                GL_UNSIGNED_BYTE, 
                imgBuffer
            );
        } else if (m_colourChannels == 1) {
            glTexImage2D
            (
                GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                NULL, 
                GL_R, 
                m_width, 
                m_height, 
                NULL, 
                GL_R, 
                GL_UNSIGNED_BYTE, 
                imgBuffer
            );
        } else {
            std::cerr << "Could not get colour channels from image: " << filePaths[i] << '\n';
            stbi_image_free(imgBuffer);
        }

        stbi_image_free(imgBuffer);
    }

    if (effects.wrapping.repeat) {
        setTextureWrappingParams(GL_TEXTURE_CUBE_MAP, GL_REPEAT);
    } else if (effects.wrapping.mirroredRepeat) {
        setTextureWrappingParams(GL_TEXTURE_CUBE_MAP, GL_MIRRORED_REPEAT);
    } else if (effects.wrapping.clampToEdge) {
        setTextureWrappingParams(GL_TEXTURE_CUBE_MAP, GL_CLAMP_TO_EDGE);
    } else if (effects.wrapping.clampToBorder) {
        setTextureWrappingParams(GL_TEXTURE_CUBE_MAP, GL_CLAMP_TO_BORDER);
        glGetTextureParameterfv(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_BORDER_COLOR, effects.wrapping.borderColours);
    }

    if (effects.filtering.bilinear) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    } else if (effects.filtering.nearest) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}