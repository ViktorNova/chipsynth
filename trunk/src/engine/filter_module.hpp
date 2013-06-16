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

#ifndef FILTER_MODULE_H
#define FILTER_MODULE_H

#include "filter.h"

#define MIN_FILT	0
#define MAX_FILT	2047

/**
 * A wrapper for the filter class
 * provides filter cutoff sweeping
 */
class filter_module : public filter
{
private:
    int _Min;
    int _Max;
    int _val;
    int _cutoff;
    int _speed;
    int _divider;
    int _counter;

public:
    filter_module(int sampleFreq) :
        filter(sampleFreq),
        _Min(MIN_FILT),
        _Max(MAX_FILT),
        _val(0),
        _cutoff(0),
        _speed(0),
        _divider(sampleFreq / 500),
        _counter(0) {}

    void cutoff(int param)
    {
        _cutoff = param;
        filter::cutoff(_cutoff);
    }

    void depth(int param)
    {
        _Min = _cutoff - param;
        if (_Min<MIN_FILT)
            _Min = MIN_FILT;
        _Max = _cutoff + param;
        if (_Max>MAX_FILT)
            _Max = MAX_FILT;
    }

    void speed(int param) { _speed = param; }

    void start(void)
    {
        _val = _cutoff;
        filter::cutoff(_val);
        _counter = _divider;
    }

    float clock(float Vi)
    {
        _counter--;
        if (!_counter)
        {
            _counter = _divider;
            if (_speed)
            {
                _val += _speed;
                if (_val>_Max)
                    _val = _Max;
                else
                    if (_val<_Min)
                        _val = _Min;
                filter::cutoff(_val);
            }
        }
        return filter::clock(Vi);
    }
};

#endif /* FILTER_MODULE_H */
