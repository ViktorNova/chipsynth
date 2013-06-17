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

#ifndef CHIPSYNTH_H
#define CHIPSYNTH_H

#include "chipsynth_ui.h"

#include <stdint.h>

#include "alsa_sound.h"
#include "csmidi.h"
#include "sound_thread.h"

class engine;

class chipsynth : public QMainWindow, public Ui::chipsynth
{
    Q_OBJECT

public:
    chipsynth(QWidget* parent = 0, Qt::WindowFlags fl = Qt::Window);
    ~chipsynth();

public slots:
    void setVolume(int param);
    void setLP();
    void setBP();
    void setHP();
    void setCutoff(int param);
    void setRes(int param);
    void openVkeyb();
    void openAbout();
    void setAttack(int param);
    void setDecay(int param);
    void setSustain(int param);
    void setRelease(int param);
    void setTriangle();
    void setSawtooth();
    void setPulse();
    void setNoise();
    void setPW(int param);
    void setSync();
    void setRing();
    void setArp();
    void setGliss();
    void setFsDepth(int param);
    void setFsSpeed(int param);
    void setSplit(int param);
    void setVoice1();
    void setVoice2();
    void setPmSpeed(int param);
    void setPmDepth(int param);
    void setVibSpeed(int param);
    void setEchoTime(int param);
    void setEchoFbk(int param);
    void setEchoMix(int param);
    void setChorusRate( int param);
    void setChorusFbk( int param );
    void setChorusMix(int param);
    virtual void displayParams( int v_num );
    void reset();
    void config();
    void setFiltAll();
    void setFiltEcho();

protected slots:
    virtual void languageChange();

private:
    void init();
    void destroy();

private:
    struct
    {
        int     volume;
        int     filt_type;
        int     filt_cutoff;
        int     filt_res;
        uint16_t adsr;
        uint16_t waveform;
        int     control;
        bool    sync;
        bool    ring_mod;
        int     vib_speed;
        int     fs_depth;
        int     fs_speed;
        uint16_t pm;
        uint16_t echo;
        uint16_t chorus;
    } voi_dat[2];

    csMidi *aMid;
    alsaAudio *aAud;

    engine *eng;

    int ctrl;
    int filt;
    int voi_num;

//bool            vibrato, filtSweep, pwMod;
//int             pulseWidth, filterCutoff;

    audio_thread audio;
};

#endif // CHIPSYNTH_H
