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

#include "engine.h"

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
