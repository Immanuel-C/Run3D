#pragma once

#include <string>

#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>
#include <unordered_map>

#include "stb_vorbis.h"

struct SoundData {
    short* buffer = nullptr;
    int format = 0, bufferSize = 0, sampleRate = 0, channels = 0;
    std::string filePath;
};

class Sound {
public:
    Sound(const std::string& filePath);
    void play(bool doesLoop, float gain = 1.0f);
    void stop();
    void pause();

    ~Sound() { destroy(); }

    void destroy();
private:
    static ALCcontext* m_audioContext;
    static ALCdevice* m_audioDevice;

    uint32_t m_sourceID;
    uint32_t m_bufferID;

    std::string m_filePath = "";

    static std::unordered_map<std::string, SoundData> m_cachedSounds;
};
