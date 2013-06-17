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

#ifndef CSMIDI_H
#define CSMIDI_H

#include "RtMidi.h"

class chipsynth;
class engine;

class csMidi
{
private:
    chipsynth *_cs;
    engine *_eng;
    RtMidiIn *_midiin;

public:
    csMidi(chipsynth *cs, engine *eng) :
        _cs(cs),
        _eng(eng),
        _midiin(new RtMidiIn()) {}
    ~csMidi() { delete _midiin; }

    bool open();
    void close() { _midiin->closePort(); }
    int portId() const { return 0; }
    int clientId() const { return 0; }
};

#endif
