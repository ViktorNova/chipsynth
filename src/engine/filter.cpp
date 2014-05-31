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

#include "filter.h"

#define VOLUME		(_mode_vol & 0x0F)

float filter::clock(float Vi)
{
    _Vlp -= _w0 * _Vbp;
    _Vbp -= _w0 * _Vhp;
    _Vhp = _Vbp/_Q - _Vlp;
    if (_mode_vol & 0xF0)
            _Vhp -= Vi;

    float Vo;
    /* output appropriate values depending on mode */
    switch (_mode_vol & 0xF0)
    {
    case 0x00:
        Vo = Vi;
        break;
    case 0x10:
        Vo = _Vlp;
        break;
    case 0x20:
        Vo = _Vbp;
        break;
    case 0x30:
        Vo = _Vlp + _Vbp;
        break;
    case 0x40:
        Vo = _Vhp;
        break;
    case 0x50:
        Vo = _Vlp + _Vhp;
        break;
    case 0x60:
        Vo = _Vbp + _Vhp;
        break;
    case 0x70:
        Vo = _Vlp + _Vbp + _Vhp;
        break;
    }

    return (Vo * VOLUME) / 15.0f;
}
