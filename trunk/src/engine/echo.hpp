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

#ifndef ECHO_H
#define ECHO_H

#include <stdint.h>

#include "delay.hpp"

/**
 This class implements an echo
 Parameters:
        time            0 - 255
        feedback        0 - 15
        mix             0 - 15
*/
class echo
{
public:
    static const uint16_t TIME_MASK = 0xFF00;
    static const uint16_t FBK_MASK = 0x00F0;
    static const uint16_t MIX_MASK = 0x000F;

public:
    static uint16_t TIME(uint16_t x) { return (x & TIME_MASK) >> 8; }
    static uint16_t FBK(uint16_t x) { return (x & FBK_MASK) >> 4; }
    static uint16_t MIX(uint16_t x) { return x & MIX_MASK; }

private:
    delay* _del;

    /** delay line output */
    uint32_t _delOut;

    uint16_t _param;

public:
    echo(int sampleFreq) :
        _del(new delay(sampleFreq, 512)),
        _delOut(0),
        _param(0) {}

    ~echo() { delete _del; }

    void parameter(uint16_t param)
    {
        _param = param;
        _del->time_ms(TIME(_param)<<1);
    }

    uint32_t clock(uint32_t input)
    {
        /* send input + feedback to the delay line */
        _delOut = _del->clock(input + ((_delOut * FBK(_param)) >> 4));

        /* send out input + delay output */
        return ((_delOut * MIX(_param)) >> 4);
    }
};

#endif /* ECHO_H */
