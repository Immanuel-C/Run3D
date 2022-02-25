#include "Sound.h"


ALCcontext* Sound::m_audioContext = nullptr;
ALCdevice* Sound::m_audioDevice = nullptr;

void checkError() {
    int err = alGetError();
    if (err != AL_NO_ERROR) {
        std::cerr << "OpenAL Error: " << err << '\n';
    } 
}

Sound::Sound(const std::string& filePath) {
    if (m_audioDevice == nullptr) {
        m_audioDevice = alcOpenDevice(NULL); // Get default device
        checkError();
    }

    if (m_audioDevice == nullptr) {
        std::cerr << "No sutiable audio devices or platform is unsupported!";
    }

    if (!m_audioContext) {
        m_audioContext = alcCreateContext(m_audioDevice, NULL);
        checkError();
        if (!alcMakeContextCurrent(m_audioContext)) {
            std::cout << "Failed to make context current\n";
            return;
        }
    }

    alGenSources(1, &m_sourceID);
    checkError();

    alSourcef(m_sourceID, AL_PITCH, 1.0f);
    checkError();

    alSource3f(m_sourceID, AL_POSITION, 0.0f, 0.0f, 0.0f);
    checkError();

    alSource3f(m_sourceID, AL_VELOCITY, 0.0f, 0.0f, 0.0f);
    checkError();


    alGenBuffers(1, &m_bufferID);
    checkError();

    short* audioBuffer = nullptr;
    int audioFormat = 0, audioBufferSize = 0, audioSampleRate = 0, audioChannels = 0;

    audioBufferSize = stb_vorbis_decode_filename(filePath.c_str(), &audioChannels, &audioSampleRate, &audioBuffer);
    checkError();

    if (audioBuffer == nullptr) {
        std::cerr << "Failed to load audio file: " << filePath;
    }

    if (audioChannels > 1) {
        audioFormat = AL_FORMAT_STEREO16;
    } else {
        audioFormat = AL_FORMAT_MONO16;
    }

    // Have to mutiply by 4 because OpenAL excpects a buffer size that is a multiple of 4
    alBufferData(m_bufferID, audioFormat, audioBuffer, audioBufferSize * 4, audioSampleRate);
    checkError();

    alSourcei(m_sourceID, AL_BUFFER, m_bufferID);
    checkError();
}

void Sound::play(bool doesLoop, float gain) {
    alSourcei(m_sourceID, AL_LOOPING, doesLoop == true ? AL_TRUE : AL_FALSE);
    checkError();
    
    alSourcef(m_sourceID, AL_GAIN, gain);
    checkError();

    alSourcePlay(m_sourceID);
    checkError();
}

void Sound::stop() {
    alSourceStop(m_sourceID);
    checkError();
}

void Sound::pause() {
    alSourcePause(m_sourceID);
    checkError();
}