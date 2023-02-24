//
// Created by william on 24/02/23.
//

#include "OSV/audio/Music.h"

Music::Music(const char *audioFile) : audioFile(audioFile) {}

void Music::play() {
    device = alcOpenDevice(NULL);
    context = alcCreateContext(device, NULL);
    alcMakeContextCurrent(context);

    SF_INFO fileInfo;
    SNDFILE* file = sf_open(audioFile, SFM_READ, &fileInfo);
    if (!file) {
        std::cerr << "Failed to open file: " << audioFile << std::endl;
        return;
    }
    std::vector<ALshort> data(fileInfo.frames * fileInfo.channels);
    sf_read_short(file, &data[0], data.size());
    sf_close(file);


    alGenBuffers(1, &buffer);
    ALenum format = (fileInfo.channels == 1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16;
    alBufferData(buffer, format, &data[0], data.size() * sizeof(ALshort), fileInfo.samplerate);


    alGenSources(1, &source);
    alSourcei(source, AL_BUFFER, buffer);

    alSourcePlay(source);
}

void Music::stop() {
    alDeleteSources(1, &source);
    alDeleteBuffers(1, &buffer);
    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

bool Music::isPlaying() {
    ALint state;
    alGetSourcei(source, AL_SOURCE_STATE, &state);
    return state == AL_PLAYING;
}