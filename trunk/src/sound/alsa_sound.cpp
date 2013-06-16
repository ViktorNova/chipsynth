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

#include "config.h"

#include "alsa_sound.h"

#include <iostream>

#include "chipsynth.h"
#include "engine.h"

bool alsaMidi::midiOpen()
{
    if (snd_seq_open(&seq_handle, "default", SND_SEQ_OPEN_INPUT, 0) < 0)
    {
        std::cerr << "Error opening ALSA sequencer." << std::endl;
        exit(1);
    }

    snd_seq_set_client_name(seq_handle, PACKAGE);
    _midi_port_id = snd_seq_create_simple_port(seq_handle, PACKAGE,
        SND_SEQ_PORT_CAP_WRITE|SND_SEQ_PORT_CAP_SUBS_WRITE,
        SND_SEQ_PORT_TYPE_APPLICATION);
    if (_midi_port_id < 0)
    {
        std::cerr << "Error creating sequencer port." << std::endl;
        exit(1);
    }

    _midi_client_id = snd_seq_client_id(seq_handle);

    return true;
}

void alsaMidi::midiClose()
{
    snd_seq_close(seq_handle);
    snd_seq_delete_simple_port(seq_handle, _midi_port_id);
}

void alsaMidi::midiRead()
{
    if (snd_seq_event_input_pending(seq_handle, 1) == 0) // FIXME
    {
        usleep(1000);
        return;
    }

    snd_seq_event_t *ev;

    // fetch midi event
    snd_seq_event_input(seq_handle, &ev);

    // many devices send a 'note on' event with velocity 0 rather than a distinct 'note off' event
    if ((ev->type == SND_SEQ_EVENT_NOTEON) && (ev->data.note.velocity == 0)) 
        ev->type = SND_SEQ_EVENT_NOTEOFF;

    switch (ev->type) {
    case SND_SEQ_EVENT_PITCHBEND:
        //pitch = (float)ev->data.control.value / 8192.0;
        _eng->param(ev->data.note.channel, PitchBend, ev->data.control.value);
        break;
    case SND_SEQ_EVENT_CONTROLLER:
        switch (ev->data.control.param)
        {
        case 1:		// modulation (vibrato?)
            _cs->setVibSpeed(ev->data.control.value);
            _cs->vibSpdKnob->setValue(ev->data.control.value);
            break;
        case 7:		// volume
            _cs->setVolume(ev->data.control.value / 8);
            _cs->volumeKnob->setValue(ev->data.control.value / 8);
            break;
        case 10:	// panning
            break;
        case 11:	// expression
            break;
        case 64:	// sustain
            _cs->setSustain(ev->data.control.value / 8);
            _cs->sustainKnob->setValue(ev->data.control.value / 8);
            break;
        case 66:	// sostenuto
            break;
        case 91:	// reverb
            break;
        case 93:	// chorus
            break;
        }
        break;
    case SND_SEQ_EVENT_NOTEON:
        _eng->noteOn(ev->data.note.channel, ev->data.note.note);
        break;
    case SND_SEQ_EVENT_NOTEOFF:
        _eng->noteOff(ev->data.note.channel, ev->data.note.note);
        break;
    }
    snd_seq_free_event(ev);
}

/* */
bool alsaAudio::audioOpen(const char *pcm_name, unsigned int samplerate, unsigned int channels)
{
    snd_pcm_hw_params_t *hw_params;

    if (snd_pcm_open(&pcm_handle, pcm_name, SND_PCM_STREAM_PLAYBACK, 0) < 0) {
        std::cerr <<  "cannot open audio device " << pcm_name << std::endl;
        exit (1);
    }

    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(pcm_handle, hw_params);
    snd_pcm_hw_params_set_access(pcm_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(pcm_handle, hw_params, SND_PCM_FORMAT_FLOAT_LE);
    snd_pcm_hw_params_set_rate(pcm_handle, hw_params, samplerate, 0);
    snd_pcm_hw_params_set_channels(pcm_handle, hw_params, channels);
    snd_pcm_hw_params_set_periods(pcm_handle, hw_params, 5, 0);
    
    _bufsize = (samplerate / 50) * channels;
    snd_pcm_hw_params_set_period_size(pcm_handle, hw_params, _bufsize, 0);
    snd_pcm_hw_params(pcm_handle, hw_params);

    buf = new float[_bufsize];

    return true;
}

void alsaAudio::audioClose()
{
    snd_pcm_close(pcm_handle);
    delete buf;
}

void alsaAudio::audioWrite()
{
    // fill the buffer
    for (int i=0; i<_bufsize; i++)
    {
        // resampling
        for (int l=1; l<_cnt; l++)
        {
            _eng->clock();
        }
        buf[i] = _eng->clock();
    }

    while (snd_pcm_writei(pcm_handle, buf, _bufsize) < 0)
    {
        snd_pcm_prepare(pcm_handle);
        std::cerr << "buffer underrun\n" << std::endl;
    }
}
