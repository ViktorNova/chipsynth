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

#ifndef CHORUS_H
#define CHORUS_H

#include <stdint.h>

#include "delay.hpp"

#define CHORUS_RATE_MASK	0xFF00
#define CHORUS_FBK_MASK		0x00F0
#define CHORUS_MIX_MASK		0x000F

#define CHORUS_RATE(x)		((x & CHORUS_RATE_MASK) >> 8)
#define CHORUS_FBK(x)		((x & CHORUS_FBK_MASK) >> 4)
#define CHORUS_MIX(x)		(x & CHORUS_MIX_MASK)

/**
 This class implements a chorus/flanger
 Parameters:
        rate            0 - 255
        mix             0 - 15
        feedback        0 - 15
*/
class chorus 
{
private:
    delay* _del;

    /** sample rate */
    int _sr;

    uint32_t _accumulator;
    uint32_t _delOut;
    uint16_t _baseDelay;
    uint16_t _param;

public:
    chorus(int sampleFreq) :
        _del(new delay(sampleFreq, 35)),
        _sr(sampleFreq),
        _accumulator(0),
        _delOut(0),
        _baseDelay(sampleFreq / 1000),
        _param(0) {}

    ~chorus() { delete _del; }

    void parameter(uint16_t param)
    {
        _param = param;
        if (_param & 0x00F0)
            /* flanger */
            _baseDelay = _sr / 1000;
        else
            /* chorus */
            _baseDelay = _sr / 50;
    }

    uint32_t clock(uint32_t input)
    {
        _accumulator += (uint32_t)CHORUS_RATE(_param);
        _accumulator &= 0x01FFFFFF;
        uint16_t tri = (uint16_t)(_accumulator >> 13);
        tri = (((tri & 0x0800) ? (tri ^ 0x07FF) : tri) & 0x07FF); // tri is now 11 bit (0 - 2047)

        _del->time_fr(tri + _baseDelay);
        _delOut = _del->clock(input + ((_delOut * CHORUS_FBK(_param)) >> 4));
        return ((_delOut * CHORUS_MIX(_param)) >> 4);
    }
};

#endif // CHORUS_H
