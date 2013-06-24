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

#ifndef ENGINE_H
#define ENGINE_H

#include "oscillator_module.hpp"
#include "wavegen_module.hpp"
#include "envelope.h"
#include "filter_module.hpp"

#include "chorus.hpp"
#include "echo.hpp"

typedef enum
{
    Volume,             /* 0-15 */
    Filter_mode,        /* [lp/bp/hp] */
    Filter_cutoff,      /* 0-2047 */
    Filter_resonance,   /* 0-15 */
    Adsr,
    Waveform,
    Control,            /* [none-gliss-arp] */
    Ring_mod,           /* true/false */
    Sync,               /* true/false */
    //Vibrato,          /* true/false */
    //Vibrato_depth,    /* */
    Vibrato_speed,      /* */
    //Filter_sweep,     /* true/false */
    //Filter_sweep_pingpong,/* */
    Filter_sweep_depth, /* 0-2047 */
    Filter_sweep_speed, /* */
    PwMod,
    Echo,
    Chorus
    //
    ,PitchBend
} param_t;

typedef struct
{
    oscillator_module   *osc;
    wavegen_module      *wav;
    envelope            *env;
    filter_module       *filt;
    chorus              *fxChorus;
    echo                *fxEcho;
    uint8_t             ctrl;
    bool                sync;
} voice_t;

class engine
{
private:
    static const unsigned int VOICES = 2;
    static const unsigned int MAXNOTES = 6;

public:
    static const unsigned int CLOCK = 192000;

    static const int NONE = 0;
    static const int ARP =  1;
    static const int GLISS = 2;

private:
    typedef struct
    {
        int notes[MAXNOTES];
        unsigned int poly;
        unsigned int note_on;
    } note_data_t;

private:
    voice_t _voices[VOICES+1];
    note_data_t _noteData[VOICES];

    float _Vo;
    float _k;
    float _xVhp;
    float _xVlp;

    int _divider;
    int _note[VOICES];

    /* frequencies of the lower octeve */
    static float _note_freq[12];

public:
    engine();
    ~engine();

    void param(int voice, param_t param, int val = 0);
    //void bend(int voice, int param) { _pitchBend = param; };
    void noteOn(int voice, int note);
    void noteOff(int voice, int note);
    float clock(void);
};

#endif /* ENGINE_H */
