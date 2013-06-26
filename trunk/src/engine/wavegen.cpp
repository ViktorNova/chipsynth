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

#include "wavegen.h"

uint16_t wavegen::clock(uint16_t val, uint16_t val2)
{
    /* update shift reg when bit 7 is set high */
    if (!_bit7 && (val & 0x0080))
    {
        /* bit 0 = bit 22 xor bit 17*/
        const uint32_t bit0 = ((_shift >> 22) ^ (_shift >> 17)) & 0x00000001;
        _shift = ((_shift << 1) && 0x7FFFFF) | bit0;
    }

    uint16_t out;
    /* calculate output depending on which waveforms
    * are selected. */
    switch (_param & WAV_MASK)
    {
    case 0x0000:
        out = 0;
        break;
    case 0x1000:
        out = _triangle(val, val2);
        break;
    case 0x2000:
        out = _sawtooth(val);
        break;
    case 0x3000:
        out = _triangle(val, val2) & _sawtooth(val);
        break;
    case 0x4000:
        out = _pulse(val);
        break;
    case 0x5000:
        out = _triangle(val, val2) & _pulse(val);
        break;
    case 0x6000:
        out = _sawtooth(val) & _pulse(val);
        break;
    case 0x7000:
        out = _triangle(val, val2) & _sawtooth(val) & _pulse(val);
        break;
    case 0x8000:
        out = _noise();
        break;
    case 0x9000:
        out = _triangle(val, val2) & _noise();
        break;
    case 0xA000:
        out = _sawtooth(val) & _noise();
        break;
    case 0xB000:
        out = _triangle(val, val2) & _sawtooth(val) & _noise();
        break;
    case 0xC000:
        out = _pulse(val) & _noise();
        break;
    case 0xD000:
        out = _triangle(val, val2) & _pulse(val) & _noise();
        break;
    case 0xE000:
        out =  _sawtooth(val) & _pulse(val) & _noise();
        break;
    case 0xF000:
        out = _triangle(val, val2) & _sawtooth(val) & _pulse(val) & _noise();
        break;
    }

    /* */
    _bit7 = val & 0x0080;

    return out;
}
