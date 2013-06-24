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

#ifndef OSCILLATOR_MODULE_H
#define OSCILLATOR_MODULE_H

#include <stdint.h>

#include "oscillator.hpp"

/**
 * A wrapper for the oscillator class
 * provides glissato and vibrato modes
 */
class oscillator_module : public oscillator
{
private:
    const int _divider;

    int _counter;
    int _f;
    int _bend;
    uint32_t _temp;
    uint32_t _freq;
    uint32_t _acc;
    bool _gliss;
    bool _do_vibrato;


public:
    oscillator_module(int sampleFreq) :
        oscillator(),
        _divider(sampleFreq / 5000),
        _counter(_divider),
        _f(0),
        _bend(0),
        _temp(0),
        _freq(0),
        _acc(128 << 12),
        _gliss(false),
        _do_vibrato(true) {}

    void gliss(bool param) { _gliss = param; }

    void frequency(uint32_t param)
    {
        if (_freq)
            _temp = _freq;
        else
            _temp = param;
        _freq = param;
        _acc = 128 << 12;
        if (!_gliss)
            oscillator::frequency(_freq);
    }

    void speed(int param)
    {
        _f = param & 0x7F;
        if (!_f)
            oscillator::frequency(_freq);
    }

    void doVibrato(bool param) { _do_vibrato = param; }

    void bend(int param) { _bend = param; }

    uint16_t clock()
    {
        _acc += _f;
        _acc &= 0x1FFFFF; // 21 bit
        _counter--;
        if (!_counter)
        {
            _counter = _divider;
            if (_gliss)
            {
                if (_temp<_freq)
                    _temp += _temp >> 10;
                else
                    if (_temp>_freq)
                        _temp -= _temp >> 10;
            }
            else
                _temp = _freq;

            uint32_t val;
            if (_f && _do_vibrato)
            {
                const int o = (((_acc & 0x100000) ? ~_acc : _acc) >> 12) & 0xFF;
                const float osc = (float)(o + 3968);
                val = (uint32_t)(((float)_temp * osc) / 4096.0f);
            }
            else
                val = _temp;

            val += _bend << 8;
            oscillator::frequency(val);
        }
        return oscillator::clock();
    }
};

#endif /* OSCILLATOR_MODULE_H */
