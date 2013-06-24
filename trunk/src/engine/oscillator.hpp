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

#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include <stdint.h>

/**
 This class implements a 32 bit programmable phase accumulating oscillator
*/
class oscillator
{
private:
    static const uint32_t MSB32 = 0x80000000;

private:
    uint32_t _accumulator;
    uint32_t _frequency;
    uint32_t _zero;

public:
    oscillator() :
        _accumulator(0),
        _frequency(0),
        _zero(0) {}

    /* */
    void frequency(uint32_t param) { _frequency = param; }

    /** reset the accumulator */
    void reset() { _accumulator = 0; }

    /* */
    bool sync() { return (bool)_zero; }

    /* */
    uint16_t clock()
    {
        _zero = (_accumulator & MSB32);
        _accumulator += _frequency;
        _zero = _zero && !(_accumulator & MSB32);
        return (uint16_t)(_accumulator >> 20);
    }
};

#endif /* OSCILLATOR_H */
