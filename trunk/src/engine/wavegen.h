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

#ifndef WAVEGEN_H
#define WAVEGEN_H

#include <stdint.h>

#define NSEED	0x7FFFF8
#define MSB12	0x0800

#define TRI	0x1000
#define SAW	0x2000
#define PUL	0x4000
#define NOI	0x8000

#define WAV_MASK	0xF000
#define PW_MASK		0x0FFF

#define PULSEWIDTH(x)	(x & PW_MASK)

/**
 This class implements a waveform generator
 Avaiable waveforms are: triangle, sawtooth, pulse and noise
 It's possible to select multiple waveforms, the resulting
 combination is the anding of selected waveforms
*/
class wavegen
{
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
    uint16_t _noise(uint32_t s)
    {
        /* take upper 12 bit of the shift register */
        return (uint16_t)(s >> 11);
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
