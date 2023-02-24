//
// Created by william on 24/02/23.
//

#ifndef OSVENGINE_MUSIC_H
#define OSVENGINE_MUSIC_H

#include <iostream>
#include <vector>
#include <AL/al.h>
#include <AL/alc.h>
#include <sndfile.h>

class Music {
    const char* audioFile;

    ALCdevice* device;
    ALCcontext* context;

    ALuint buffer;
    ALuint source;

public:
    Music(const char* audioFile);

    void play();

    void stop();

    bool isPlaying();
};

#endif //OSVENGINE_MUSIC_H
