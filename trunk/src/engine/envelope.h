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

#ifndef ENVELOPE_H
#define ENVELOPE_H

#include <stdint.h>

/**
 This class implements an envelope generator
*/
class envelope
{
private:
    typedef enum {ATT, DEC, SUS} env_state_t;

public:
    static const uint16_t ATTACK_MASK = 0xF000;
    static const uint16_t DECAY_MASK = 0x0F00;
    static const uint16_t SUSTAIN_MASK = 0x00F0;
    static const uint16_t RELEASE_MASK =  0x000F;

public:
    static uint16_t ATTACK(uint16_t x) { return (x & ATTACK_MASK) >> 12; }
    static uint16_t DECAY(uint16_t x) { return (x & DECAY_MASK) >> 8; }
    static uint16_t SUSTAIN(uint16_t x) { return (x & SUSTAIN_MASK) >> 4; }
    static uint16_t RELEASE(uint16_t x) { return x & RELEASE_MASK; }

private:
    /** State of the envelope generator
     * Attack, Decay or Sustain */
    env_state_t _state;

    /** Attack / Decay / Sustain / Release */
    uint16_t _adsr;

    /** envelope value (0 - 255) */
    uint8_t _envel;

    /** Envelope clock divider */
    uint16_t _divider;

    /** aux vars for exponential decay simulation */
    uint8_t _exp_cnt;
    uint8_t _exp_rate;

    /* */
    bool _gate;
    bool _do_release;

    /** Envelope times in ms */
    static uint16_t _env_time[16];
    uint16_t _rate[16];


public:
    envelope(int sampleFreq);

    void adsr(uint16_t param) { _adsr = param; }
    void doRelease(bool param) { _do_release = param; }
    
    void start()
    {
        _gate = true;
        _state = ATT;
        _divider = _rate[ATTACK(_adsr)];
    }

    void stop()
    {
        _gate = false;
        _divider = _rate[RELEASE(_adsr)];
        _exp_cnt = 1;
        _exp_rate = 1;
    }

    uint8_t clock();

};

#endif /* ENVELOPE_H */
