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
 * along with this program.  If not, see <http:*www.gnu.org/licenses/>.
 */

#ifndef CSAUDIO_H
#define CSAUDIO_H

class engine;
class RtAudio;

class csAudio
{
public:
   struct userData_t
   {
        engine *eng;
        int cnt;

        userData_t(int c, engine *e) :
            eng(e),
            cnt(c) {}
   };

private:
    RtAudio *_audio;
    userData_t _userData;

public:
    csAudio(int count, engine *eng);
    ~csAudio();

    bool open(unsigned int deviceId, unsigned int samplerate, unsigned int channels);
    void close();
};

#endif
