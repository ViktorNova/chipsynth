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

#ifndef SOUND_THREAD_H
#define SOUND_THREAD_H

#include <QThread>

#include "alsa_sound.h"

class audio_thread : public QThread
{
    Q_OBJECT

protected:
    void run();

private:
    volatile bool is_running;
    alsaAudio *audio;

public:
    void interrupt() { is_running = false; }
    void setThread(alsaAudio *a) { audio = a; is_running = true; }
};

#endif // SOUND_THREAD_H
