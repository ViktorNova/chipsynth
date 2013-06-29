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
        const uint32_t bit0 = ((_shift << 22) ^ (_shift << 17)) & 0x800000;
        _shift = (_shift >> 1) | bit0;
    }

    /* Save buit 7 value for next cycle */
    _bit7 = val & 0x0080;

    uint16_t out = 0;

    if (_param & WAV_MASK)
    {
        out = 0xFFF;

        if (_param & (TRI | SAW))
        {
            const uint16_t osc =  _osc(val, val2);

            if (_param & TRI) out &= (osc << 1);
            if (_param & SAW) out &= osc;
        }

        if (_param & PUL) out &= _pulse(val);
        if (_param & NOI) out &= _noise();
    }
#if 0
    /* Feed output back into shift register 
     * if multiple waveforms selected with noise */
    if ((_param & WAV_MASK) > NOI)
    {
        const uint32_t waveform_output = (uint32_t)out;
        _shift &=
            ~((1 << 2) | (1 << 4) | (1 << 8) | (1 << 11) | (1 << 13) | (1 << 17) | (1 << 20) | (1 << 22)) |
            ((waveform_output & 0x800) >> 9) |  // Bit 11 -> bit 20
            ((waveform_output & 0x400) >> 6) |  // Bit 10 -> bit 18
            ((waveform_output & 0x200) >> 1) |  // Bit  9 -> bit 14
            ((waveform_output & 0x100) << 3) |  // Bit  8 -> bit 11
            ((waveform_output & 0x080) << 6) |  // Bit  7 -> bit  9
            ((waveform_output & 0x040) << 11) |  // Bit  6 -> bit  5
            ((waveform_output & 0x020) << 15) |  // Bit  5 -> bit  2
            ((waveform_output & 0x010) << 18);   // Bit  4 -> bit  0
    }
#endif
    return out;
}
