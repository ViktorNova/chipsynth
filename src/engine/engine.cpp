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


#include "engine.h"

// Frequency (Hz) = k*2^n/12
// where n is the number of steps along the chromatic scale, and k is a constant which specifies the starting frequency of the scale; e.g. 440Hz
float engine::_note_freq[] =
{
    16.3515625f,
    17.32390625f,
    18.35400625f,
    19.44546875f,
    20.60171875f,
    21.82679688f,
    23.1246875f,
    24.4996875f,
    25.9565625f,
    27.5f,
    29.13523438f,
    30.86773438f
};

// Bohlen-Pierce scale
// Frequency (Hz) = k*3^n/13
// where n is the number of steps along the tritave scale, and k is a constant, defined here as 220Hz

//#include <iostream> /**/
//fprintf(stderr,"note %d - %f\n",note,fr);

/*
 * Constructor
 */
engine::engine()
{
    _k = 4294967296.0f / CLOCK;
    _divider = CLOCK / 50;

    for (unsigned int i=0; i<=VOICES; i++)
    {
        _voices[i].osc = new oscillator_module(CLOCK);
        _voices[i].wav = new wavegen();
        _voices[i].env = new envelope(CLOCK);
        _voices[i].filt = new filter_module(CLOCK);
        _voices[i].mod = new oscillator();
        if (i<VOICES)
        {
            _voices[i].fxChorus = new chorus(CLOCK);
            _voices[i].fxEcho = new echo(CLOCK);
            _voices[i].ctrl = 0;
            _noteData[i].poly = 0;
            _noteData[i].note_on = 0;
        }
        else
        {
            _voices[i].fxChorus = 0;
            _voices[i].fxEcho = 0;
        }
    }

    _xVhp = 0.0f;
    _xVlp = 0.0f;
}

/*
 * Destructor
 */
engine::~engine()
{
    for (unsigned int i=0; i<=VOICES; i++)
    {
        delete _voices[i].osc;
        delete _voices[i].wav;
        delete _voices[i].env;
        delete _voices[i].filt;
        delete _voices[i].mod;
        if (i<VOICES)
        {
            delete _voices[i].fxChorus;
            delete _voices[i].fxEcho;
        }
    }
}

/*
 * Set a parameter
 */
void engine::param(int voice_num, param_t param, int val)
{
    voice_t *v = &_voices[voice_num];

    switch (param)
    {
    case Volume:
        v->filt->volume(val);
        break;
    case Filter_mode:
        v->filt->mode(val);
        break;
    case Filter_cutoff:
        v->filt->cutoff(val);
        break;
    case Filter_resonance:
        v->filt->resonance(val);
        break;
    case Adsr:
        v->env->adsr(val);
        break;
    case Waveform:
        v->wav->waveform(val);
        break;
    case Control:
        v->ctrl = val;
        if (val == GLISS)
            v->osc->gliss(true);
        else
            v->osc->gliss(false);
        if (val == ARP)
        {
            v->env->doRelease(false);
            v->osc->doVibrato(false);
        }
        else
        {
            v->env->doRelease(true);
            v->osc->doVibrato(true);
        }
        break;
    case Sync:
        v->sync = val;
        break;
    case Ring_mod:
        v->wav->ringMod(val);
        break;
    case Mod_Speed:
        v->mod->frequency(val);
        break;
    case Echo:
        if (v->fxEcho)
            v->fxEcho->parameter(val);
        break;
    case Chorus:
        if (v->fxChorus)
            v->fxChorus->parameter(val);
        break;
    case PitchBend:
        v->osc->bend(val);
        break;
    case Modulation:
        v->modulation=(modulation_t)val;
        break;
    }
}

void engine::noteOn(int voice, int note)
{
    note_data_t *nd = &_noteData[voice];

    if (nd->poly < MAXNOTES)
    {
        nd->notes[nd->poly] = note;
        nd->poly++;
    }

    _voices[voice].env->start();
   // _voices[voice].filt->start();
   // _voices[voice].wav->start();
}

void engine::noteOff(int voice, int note)
{
    note_data_t	*nd = &_noteData[voice];

    for (unsigned int i=0; i<nd->poly; i++)
    {
        if (nd->notes[i] == note)
        {
            for (unsigned int j=i; j<nd->poly-1; j++)
            {
                nd->notes[j] = nd->notes[j+1];
            }
            nd->poly--;
        }
    }

    if (!nd->poly)
    {
        _voices[voice].env->stop();
        //if (voices[voice_num].ctrl != GLISS)
        //	voices[voice_num].osc->setFreq(0);
    }

    _divider = 1;
}

float engine::clock(void)
{
    /* set the frequency register according to note played */
    _divider--;
    for (unsigned int i=0; i<VOICES; i++)
    {
        note_data_t *nd = &_noteData[i];
        if (nd->poly > 0)
        {
            /* one or more notes playing */
            if (_voices[i].ctrl & ARP)
            {
                /* arpeggio mode */
                if (!_divider)
                {
                    if (nd->note_on >= nd->poly)
                        nd->note_on = 0;
                    _note[i] = nd->notes[nd->note_on];
                    const float fr = (_note_freq[_note[i]%12] * (1 << (_note[i]/12)));
                    _voices[i].osc->frequency((uint32_t)(fr * _k));
                    nd->note_on++;
                }
            }
            else
            {
                /* non arpeggio mode */
                if (_note[i] != nd->notes[0])
                {
                    _note[i] = nd->notes[0];
                    const float fr = (_note_freq[_note[i]%12] * (1 << (_note[i]/12)));
                    _voices[i].osc->frequency((uint32_t)(fr * _k));
                }
            }
        }
        //else
        //{
            /* no notes playing */
            //voices[i].osc->setFreq(0);
            //voices[i].osc->reset();
        //}
    }
/*
    switch (v->modulation)
    {
    case VIBRATO:
        break;
    }
*/
    /* set drum params */
    // TODO

    float Vi = 0.0f;
    int val[VOICES+1];
    int wav[VOICES+1];

    /* clock oscillators */
    for (unsigned int i=0; i<=VOICES; i++)
        val[i] = _voices[i].osc->clock();

    /* syncronize oscillators if needed */
    for (unsigned int i=0; i<VOICES; i++)
    {
        if (_voices[i].sync)
        {
            int j = i - 1;
            if (j < 0)
                j += VOICES;
            if (_voices[j].osc->sync())
                _voices[i].osc->reset();
        }
    }

    /* generate waveforms */
    for (unsigned int i=0; i<VOICES; i++)
    {
        const int j = ((i-1)<0) ? i-1+VOICES : i-1;
        wav[i] = _voices[i].wav->clock(val[i], val[j]);
    }
    wav[VOICES] = _voices[VOICES].wav->clock(val[VOICES]);

    //Vi = Vv[VOICES];

    for (unsigned int i=0; i<=VOICES; i++)
    {
        /* clock envelope generators */
        const int env = _voices[i].env->clock();
        /* effects */
        int Vfx = wav[i] * env;
        int echoOut = 0;
        if (i<VOICES)
        {
            Vfx += _voices[i].fxChorus->clock(Vfx);
            echoOut = _voices[i].fxEcho->clock(Vfx);
        }
        /* filters */
        const float filtIn = (float)(echoOut + Vfx) / 1048576.0f;
        //float filtIn = (float)echoOut / 1048576.0;
        //Vi += (float)Vfx / 1048576.0;
        Vi += _voices[i].filt->clock(filtIn);
    }

    /* apply external filter */
    _Vo = (_xVlp - _xVhp);	//
    _xVhp += (_xVlp - _xVhp) * (100.f / CLOCK); // HP at 16Hz
    _xVlp += (Vi - _xVlp) * (100000.f / CLOCK); // LP at 16KHz

    /* modulation */

    if (!_divider)
        _divider = CLOCK / 25;

    return _Vo;
}
