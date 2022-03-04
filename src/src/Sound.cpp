#include "Sound.h"

ALCcontext* Sound::m_audioContext = nullptr;
ALCdevice* Sound::m_audioDevice = nullptr;

std::unordered_map<std::string, SoundData> Sound::m_cachedSounds;

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

    // If no audio device is avalible print error
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


    for (const auto& [key, value] : m_cachedSounds) {
        if (strcmp(key.c_str(), filePath.c_str()) == NULL) {
            // Have to mutiply by 4 because OpenAL excpects a buffer size that is a multiple of 4
            alBufferData(m_bufferID, value.format, value.buffer, value.bufferSize * 4, value.sampleRate);
            checkError();

            alSourcei(m_sourceID, AL_BUFFER, m_bufferID);
            checkError();

            return;
        } 
    }

    SoundData audioData;

    audioData.bufferSize = stb_vorbis_decode_filename(filePath.c_str(), &audioData.channels, &audioData.sampleRate, &audioData.buffer);
    
    if (audioData.buffer == nullptr) {
        std::cerr << "Failed to load audio file: " << filePath;
        return;
    }

    m_filePath = filePath;

    if (audioData.channels > 1) {
        audioData.format = AL_FORMAT_STEREO16;
    } else {
        audioData.format = AL_FORMAT_MONO16;
    }

    // Have to mutiply by 4 because OpenAL excpects a buffer size that is a multiple of 4
    alBufferData(m_bufferID, audioData.format, audioData.buffer, audioData.bufferSize * 4, audioData.sampleRate);
    checkError();

    alSourcei(m_sourceID, AL_BUFFER, m_bufferID);
    checkError();

    m_cachedSounds.insert({filePath, audioData});
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

void Sound::destroy() {
    if (strcmp(m_filePath.c_str(), "") != NULL) {
        //delete m_cachedSounds[m_filePath].buffer; // stb_vorbis allocats sound data on heap
        m_cachedSounds.erase(m_filePath);
    }
}