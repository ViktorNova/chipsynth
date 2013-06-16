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

/**
 * A wrapper for the waveform class
 * provides pulse width modulation
 */

#ifndef WAVEGEN_MODULE_H
#define WAVEGEN_MODULE_H

#include <stdint.h>

#include "wavegen.h"

#define PW_SPEED_MASK	0x000F
#define PW_DEPTH_MASK	0xFFF0

#define PW_SPEED(x)	(x & PW_SPEED_MASK)
#define PW_DEPTH(x)	(x >> 4)

class wavegen_module : public wavegen
{
private:
    int _divider;
    int _counter;

    //uint16_t _Min;
    //uint16_t _Max;
    uint16_t _pw;
    uint16_t _wav;
    uint16_t _mod;

public:
    wavegen_module(int sampleFreq) :
        wavegen(),
        _divider(sampleFreq / 500),
        _counter(_divider),
        /*_Min = MIN_WAV;
        _Max = MAX_WAV;*/
        _pw(0),
        _wav(0),
        _mod(0) {};

    void waveform(uint16_t param)
    {
        _wav = param;
        wavegen::waveform(_wav);
    }

    void modulation(uint16_t param)
    {
        //uint16 pw = _wav & 0x0FFF;
        _mod = param;

        /*_Min = (pw - param) << 4;
        if (_Min<MIN_WAV)
                _Min = MIN_WAV;
        _Max = (pw + param) << 4;
        if (_Max>MAX_WAV)
                _Max = MAX_WAV;*/
    }

    void start()
    {
        _pw = _wav << 4;
        wavegen::waveform(_wav);
        _counter = _divider;
    }

    uint16_t clock(uint16_t val1, uint16_t val2 = 0)
    {
        _counter--;
        if (!_counter)
        {
            _counter = _divider;
            if (PW_SPEED(_mod))
            {
                _pw += PW_SPEED(_mod);
                /*if ((_val>_Max) || (_val<_Min))
                        _speed = -_speed;*/
                wavegen::waveform((_pw >> 4) | (_wav & 0xF000));
            }
        }
        return wavegen::clock(val1, val2);
    }
};

#endif /* WAVEGEN_MODULE_H */
