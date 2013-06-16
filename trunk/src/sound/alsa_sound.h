/*
 * This file is part of ChipSynth.
 *
 * Copyright 2005, 2013 Leandro Nini
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

#ifndef ALSA_SOUND_H
#define ALSA_SOUND_H

#include <alsa/asoundlib.h>

class chipsynth;
class engine;

class alsaMidi
{
private:
    snd_seq_t *seq_handle;
    chipsynth *_cs;
    engine *_eng;

    int _midi;
    int _midi_port_id;
    int _midi_client_id;

public:
    alsaMidi(chipsynth *cs, engine *eng) :
        _cs(cs),
        _eng(eng),
        _midi(-1) {}

    bool midiOpen();
    void midiClose();
    void midiRead();
    int portId() const { return _midi_port_id; }
    int clientId() const { return _midi_client_id; }
};


class alsaAudio
{
private:
    snd_pcm_t *pcm_handle;
    engine *_eng;

    float *buf;

    int _audio;
    int _cnt;
    int _rate;
    int _bufSize;
    int _stereo;
    int _format;
    unsigned int _bufsize;

public:
    alsaAudio(int count, engine *eng) :
        _eng(eng),
        _audio(-1),
        _cnt(count),
        _rate(0),
        _stereo(0),
        _format(0),
        _bufSize(0) {}

    bool audioOpen(const char *pcm_name, unsigned int samplerate, unsigned int channels);
    void audioClose();
    void audioWrite();

};

#endif /* ALSA_SOUND_H */
