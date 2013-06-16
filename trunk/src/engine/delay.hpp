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

#ifndef DELAY_H
#define DELAY_H

#include <stdint.h>

/**
 This class implements a variable length delay
*/
class delay
{
private:
    /** circular buffer for storing values */
    uint32_t* _buffer;

    /** length of the buffer */
    int _length;

    /** pointer to the input value into buffer */
    int _pointer;

    /** distance of the output value from the input value
     * calculated on the delay time */
    int _offset;

    /** sample rate */
    int _sr;

public:
    delay(int sampleFreq, int maxDelay) :
        /* calculate length of buffer
         * maxDelay is the maximum delay time needed in ms */
        _length(maxDelay * sampleFreq / 1000),
        _pointer(0),
        _offset(0),
        _sr(sampleFreq)
    {
        /* allocate buffer */
        _buffer = new uint32_t[_length];
    }

    ~delay() { delete _buffer; }

    void time_ms(int ms)
    {
        /* calculate delay offset */
        _offset = ms * _sr / 1000;
        /* delay offset cannot exceed the buffer length */
        if (_offset>_length)
            _offset = _length;
    }

    void time_fr(int fr)
    {
        if (fr<_length)
            _offset = fr;
        else
            _offset = _length;
    }

    uint32_t clock(uint32_t input)
    {
        /* store current input value */
        _buffer[_pointer] = input;

        /* calculate output value position */
        int tp = _pointer - _offset;
        if (tp<0)
            tp += _length;

        _pointer++;
        if (_pointer>_length)
            _pointer = 0;

        return _buffer[tp];
    }
};

#endif /* DELAY_H */
