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

#ifndef FILTER_H
#define FILTER_H

#include <stdint.h>

/**
 This class implements a two-integrator-loop biquadratic filter
 the code is taken from reSID but operates on floats rather than
 fixed point
*/
class filter
{
public:
    static const int LOWPASS = 0x10;
    static const int BANDPASS = 0x20;
    static const int HIGHPASS = 0x40;

private:
    /** sample rate */
    int _sr;

    float _Vlp;
    float _Vbp;
    float _Vhp;

    /** cutoff frequency
        * w0 = (2 * pi * cutoff freq) / clock
        * 31.83 < cutoff freq < 12737.65
        * 0.0002029941639 < w0 < 0.0812311596 */
    float _w0;

    /** resonance
     * 0.707 < Q < 1.707 */
    float _Q;

    /** mode / volume */
    uint8_t _mode_vol;

public:
    filter(int sampleFreq) :
        _sr(sampleFreq),
        _Vlp(0.0f),
        _Vbp(0.0f),
        _Vhp(0.0f),
        _w0(0.0f),
        _Q(0.707f),
        _mode_vol(0) {}

    void cutoff(int param) { _w0 = ((float)(param & 0x0FFF) * 39.0f + 200.0f) / _sr; }

    void resonance(int param) { _Q = 0.707f + (float)(param & 0x0F) / 15.0f; }

    void mode(int param)
    { 
        _mode_vol &= 0x0F;
        _mode_vol |= (param & 0xF0);
    }

    void volume(int param)
    {
        _mode_vol &= 0xF0;
        _mode_vol |= (param & 0x0F);
    }

    float clock(float Vi);
};

#endif /* FILTER_H */
