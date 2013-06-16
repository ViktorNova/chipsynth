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

#include "envelope.h"

/* envelope times in ms (taken from the SID) */
uint16_t envelope::_env_time[] =
{
    2,
    8,
    16,
    24,
    38,
    56,
    68,
    80,
    100,
    250,
    500,
    800,
    1000,
    3000,
    5000,
    8000
};


envelope::envelope(int sampleFreq) :
    _state(SUS),
    _adsr(0),
    _envel(0),
    _divider(0),
    _exp_cnt(0),
    _exp_rate(0),
    _gate(false),
    _do_release(true)
{
    const float k = (float)sampleFreq / 256000.0f;

    for (int i=0; i<=15; i++)
    {
            /* env_rate = (env_time/1000) * clock / 256 */
            _rate[i] = (int)((float)_env_time[i] * k);
    }
}

uint8_t envelope::clock()
{
    if (_gate)
    {
        switch (_state)
        {
        case ATT:
            /* we're in the attack state */
            _divider--;
            if (_divider == 0)
            {
                _envel++;
                if (_envel == 255)
                {
                    /* we have reached maximum value
                        * begin the decay phase */
                    _state = DEC;
                    /* with proper rate */
                    _divider = _rate[DECAY(_adsr)];
                    _exp_cnt = 1;
                    _exp_rate = 1;
                }
                else
                    _divider = _rate[ATTACK(_adsr)];
            }
            break;
        case DEC:
            /* we're in the decay state */
            _divider--;
            if (_divider == 0)
            {
                _exp_cnt--;
                _divider = _rate[DECAY(_adsr)];
                if (_exp_cnt == 0)
                {
                    _envel--;
                    if (_envel == (SUSTAIN(_adsr) << 4))
                    {
                            /* we have reached the sustain level
                                * switch to sustain state */
                            _state = SUS;
                    }
                    else
                    {
                        switch (_envel) 
                        {
                        case 93:
                        case 54:
                        case 26:
                        case 14:
                        case 6:
                            _exp_rate <<= 1;
                        }
                        _exp_cnt = _exp_rate;
                    }
                }
            }
            break;
        case SUS:
            /* we're in the sustain state */
            const uint8_t sust = (SUSTAIN(_adsr) << 4);
            if (_envel > sust)
                _envel--;
            else
                if (_envel < sust)
                    _envel++;
            break;
        }
    }
    else
    {
        /* gate off */
        if (_envel>0)
        {
            if (_do_release)
            {
                _divider--;
                if (_divider == 0)
                {
                    _exp_cnt--;
                    _divider = _rate[RELEASE(_adsr)];
                    if (_exp_cnt == 0)
                    {
                        _envel--;
                        switch (_envel)
                        {
                        case 93:
                        case 54:
                        case 26:
                        case 14:
                        case 6:
                            _exp_rate <<= 1;
                        }
                        _exp_cnt = _exp_rate;
                    }
                }
            }
            else
                _envel--;
        }
    }

    return _envel;
}
