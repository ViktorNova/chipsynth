/*
 * This file is part of ChipSynth.
 *
 * Copyright 2013 Leandro Nini
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "csaudio.h"

#include "RtAudio.h"
#include "RtError.h"
#include "engine.h"

int mycallback(void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *userData)
{
    engine *eng = static_cast<csAudio::userData_t*>(userData)->eng;
    const int cnt = static_cast<csAudio::userData_t*>(userData)->cnt;
    float *buffer = static_cast<float*>(outputBuffer);

    if (status)
        std::cout << "Stream underflow detected!" << std::endl;

    // Write interleaved audio data.
    for (unsigned int i=0; i<nBufferFrames; i++)
    {
        // resampling
        for (int l=1; l<cnt; l++)
        {
            eng->clock();
        }
        buffer[i] = eng->clock();
    }
    return 0;
}

csAudio::csAudio(int count, engine *eng) :
    _userData(count, eng)
{
    _audio = new RtAudio();
}

csAudio::~csAudio()
{
    delete _audio;
}

bool csAudio::open(unsigned int deviceId, unsigned int samplerate, unsigned int channels)
{
    if (_audio->getDeviceCount() < 1)
    {
        std::cout << "\nNo audio devices found!" << std::endl;
        return false;
    }

    RtAudio::StreamParameters parameters;
    parameters.deviceId = _audio->getDefaultOutputDevice();
    parameters.nChannels = channels;
    parameters.firstChannel = 0;
    unsigned int sampleRate = samplerate;
    unsigned int bufferFrames = 256; // 256 sample frames

    RtAudio::StreamOptions options;
    options.flags = RTAUDIO_MINIMIZE_LATENCY|RTAUDIO_SCHEDULE_REALTIME;
    options.numberOfBuffers = 2;
    options.streamName = "ChipSynth Audio Stream";

    try
    {
        _audio->openStream(&parameters, NULL, RTAUDIO_FLOAT32,
            sampleRate, &bufferFrames, &mycallback, static_cast<void*>(&_userData), &options);
        _audio->startStream();
    }
    catch (const RtError &e)
    {
        e.printMessage();
        return false;
    }

    return true;
}

void csAudio::close()
{
    try
    {
        _audio->stopStream();
    }
    catch (const RtError &e)
    {
        e.printMessage();
    }

    if (_audio->isStreamOpen())
        _audio->closeStream();
}
