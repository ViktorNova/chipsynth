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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef WAVEGEN_H
#define WAVEGEN_H

#include <stdint.h>

/**
 This class implements a waveform generator
 Avaiable waveforms are: triangle, sawtooth, pulse and noise
 It's possible to select multiple waveforms, the resulting
 combination is the anding of selected waveforms
*/
class wavegen
{
private:
    static const uint32_t NSEED = 0x7FFFFF;
    static const uint16_t MSB12 = 0x0800;

public:
    static const uint16_t TRI = 0x1000;
    static const uint16_t SAW = 0x2000;
    static const uint16_t PUL = 0x4000;
    static const uint16_t NOI = 0x8000;

    static const uint16_t PW_MASK = 0x0FFF;

private:
    static const uint16_t WAV_MASK = 0xF000;

public:
    static uint16_t PULSEWIDTH(uint16_t x) { return x & PW_MASK; }

private:
    uint16_t _param;

    /** Ring modulation */
    bool _ringMod;

    /** shift register for noise generation 
     * 23 bit */
    uint32_t _shift;

    /*  */
    uint16_t _bit7;

private:
    /* Theese functions generates the base waveforms
        * given the oscillator value */

    /** Triangle waveform
     * goes linearly from 0 to 4095 and then back 
     * with increments of 2 */
    uint16_t _triangle(uint16_t v, uint16_t v2)
    {
        const uint16_t val = (_ringMod) ? v ^ v2 : v;
        /* when the MSB is set invert the output */
        return ((((val & MSB12) ? (val ^ 0x07FF) : val) & 0x07FF) << 1);
    }

    /** Sawtooth waveform
     * goes linearly from 0 to 4095 and then wraps to 0 */
    uint16_t _sawtooth(uint16_t v)
    {
        return (v & 0x0FFF);
    }

    /** Pulse waveform */
    uint16_t _pulse(uint16_t v)
    {
        /* compare the upper 12 bit of the oscillator
         * with pulse width, if less output 0
         * otherwise output maximum value (4095) */
        return (((v & 0x0FFF) < PULSEWIDTH(_param)) ? 0x0000 : 0x0FFF);
    }

    /** Noise waveform 
     * pseudo random */
    uint16_t _noise()
    {
        /* Noise generator schematic from reSID
         *
         *                reset    -------------------------------------------
         *                  |     |                                           |
         *           test--OR-->EOR<--                                        |
         *                  |         |                                       |
         *                  2 2 2 1 1 1 1 1 1 1 1 1 1                         |
         * Register bits:   2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 <---
         *                      |   |       |     |   |       |     |   |
         * Waveform bits:       1   1       9     8   7       6     5   4
         *                      1   0
         *
         * The low 4 waveform bits are zero (grounded).
         *
         * NOTE: the bits in _shift are inverted */
        return (uint16_t)
            ((_shift & (1 << 2)) << (11 - 2)) |
            ((_shift & (1 << 4)) << (10 - 4)) |
            ((_shift & (1 << 8)) << (9 - 8)) |
            ((_shift & (1 << 11)) >> (11 - 8)) |
            ((_shift & (1 << 13)) >> (13 - 7)) |
            ((_shift & (1 << 17)) >> (17 - 6)) |
            ((_shift & (1 << 20)) >> (20 - 5)) |
            ((_shift & (1 << 22)) >> (22 - 4));
    }

public:
    wavegen() :
        _param(0),
        _ringMod(false),
        _shift(NSEED),
        _bit7(0) {}

    void waveform(uint16_t param) { _param = param; }

    void ringMod(bool param) { _ringMod = param; }

    uint16_t clock(uint16_t val, uint16_t val2 = 0);
};

#endif /* WAVEGEN_H */
