#pragma once

#include <string>

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>

#include "stb_vorbis.h"

class Sound {
public:
    Sound(const std::string& filePath);
    void play(bool doesLoop, float gain = 1.0f);
    void stop();
    void pause();
private:
    static ALCcontext* m_audioContext;
    static ALCdevice* m_audioDevice;

    uint32_t m_sourceID;
    uint32_t m_bufferID;
};
