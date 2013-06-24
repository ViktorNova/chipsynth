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

#include "chipsynth.h"

#include <QMessageBox>
#include <string.h>

#include "engine.h"
#include "configDialog.h"
#include "kled.h"

#define AUTHOR          "Copyright 2005, 2013 Leandro Nini"

/*
 *  Constructs a chipsynth as a child of 'parent', with widget flags set to 'f'.
 *
 */
chipsynth::chipsynth(QWidget* parent, Qt::WindowFlags fl) :
    QMainWindow(parent, fl),
    ctrl(0),
    filt(0),
    voi_num(0)
{
    setupUi(this);

    //(void)statusBar();
    init();
}

/*
 *  Destroys the object and frees any allocated resources
 */
chipsynth::~chipsynth()
{
    destroy();
    // no need to delete child widgets, Qt does it all for us
}

/*
 *  Sets the strings of the subwidgets using the current
 *  language.
 */
void chipsynth::languageChange()
{
    retranslateUi(this);
}

void chipsynth::setVolume(int param)
{
    eng->param(voi_num, Volume, param);
    volumeDisplay->display(param);
    voi_dat[voi_num].volume = param;
}

void chipsynth::setLP()
{
    if (filt & filter::LOWPASS)
    {
        filt &= ~filter::LOWPASS;
        lpLed->off();
    }
    else
    {
        filt |= filter::LOWPASS;
        lpLed->on();
    }

    eng->param(voi_num, Filter_mode, filt);
    voi_dat[voi_num].filt_type = filt;
}

void chipsynth::setBP()
{
    if (filt & filter::BANDPASS)
    {
        filt &= ~filter::BANDPASS;
        bpLed->off();
    }
    else
    {
        filt |= filter::BANDPASS;
        bpLed->on();
    }

    eng->param(voi_num, Filter_mode, filt);
    voi_dat[voi_num].filt_type = filt;
}

void chipsynth::setHP()
{
    if (filt & filter::HIGHPASS)
    {
        filt &= ~filter::HIGHPASS;
        hpLed->off();
    }
    else
    {
        filt |= filter::HIGHPASS;
        hpLed->on();
    }

    eng->param(voi_num, Filter_mode, filt);
    voi_dat[voi_num].filt_type = filt;
}

void chipsynth::setCutoff(int param)
{
    eng->param(voi_num, Filter_cutoff, param);
    cutoffDisplay->display(param);
    voi_dat[voi_num].filt_cutoff = param;
}

void chipsynth::setRes(int param)
{
    eng->param(voi_num, Filter_resonance, param);
    resDisplay->display(param);
    voi_dat[voi_num].filt_res = param;
}

void chipsynth::openAbout()
{
    QString title = "About "PACKAGE_NAME;
    QString message = PACKAGE_NAME" "VERSION"\n\n"AUTHOR"\n\n"PACKAGE_URL;
    QMessageBox::about(this, title, message);
}

void chipsynth::setAttack(int param)
{
    uint16_t val = voi_dat[voi_num].adsr & ~envelope::ATTACK_MASK;
    val |= (param << 12);
    eng->param(voi_num, Adsr, val);
    attackDisplay->display(param);
    voi_dat[voi_num].adsr = val;
}

void chipsynth::setDecay(int param)
{
    uint16_t val = voi_dat[voi_num].adsr & ~envelope::DECAY_MASK;
    val |= (param << 8);
    eng->param(voi_num, Adsr, val);
    decayDisplay->display(param);
    voi_dat[voi_num].adsr = val;
}

void chipsynth::setSustain(int param)
{
    uint16_t val = voi_dat[voi_num].adsr & ~envelope::SUSTAIN_MASK;
    val |= (param << 4);
    eng->param(voi_num, Adsr, val);
    sustainDisplay->display(param);
    voi_dat[voi_num].adsr = val;
}

void chipsynth::setRelease(int param)
{
    uint16_t val = voi_dat[voi_num].adsr & ~envelope::RELEASE_MASK;
    val |= param;
    eng->param(voi_num, Adsr, val);
    releaseDisplay->display(param);
    voi_dat[voi_num].adsr = val;
}

void chipsynth::setTriangle()
{
    uint16_t val = voi_dat[voi_num].waveform;
    if (val & wavegen::TRI)
    {
        val &= ~wavegen::TRI;
        triLed->off();
    }
    else
    {
        val |= wavegen::TRI;
        triLed->on();
    }

    eng->param(voi_num, Waveform, val);
    voi_dat[voi_num].waveform = val;
}

void chipsynth::setSawtooth()
{
    uint16_t val = voi_dat[voi_num].waveform;
    if (val & wavegen::SAW)
    {
        val &= ~wavegen::SAW;
        sawLed->off();
    }
    else
    {
        val|= wavegen::SAW;
        sawLed->on();
    }

    eng->param(voi_num, Waveform, val);
    voi_dat[voi_num].waveform = val;
}

void chipsynth::setPulse()
{
    uint16_t val = voi_dat[voi_num].waveform;
    if (val & wavegen::PUL)
    {
        val &= ~wavegen::PUL;
        pulLed->off();
    }
    else{
        val |= wavegen::PUL;
        pulLed->on();
    }

    eng->param(voi_num, Waveform, val);
    voi_dat[voi_num].waveform = val;
}

void chipsynth::setNoise()
{
    uint16_t val = voi_dat[voi_num].waveform;
    if (val & wavegen::NOI)
    {
        val &= ~wavegen::NOI;
        noiLed->off();
    }
    else
    {
        val |= wavegen::NOI;
        noiLed->on();
    }

    eng->param(voi_num, Waveform, val);
    voi_dat[voi_num].waveform = val;
}

void chipsynth::setPW(int param)
{
    uint16_t val = voi_dat[voi_num].waveform & ~wavegen::PW_MASK;
    val |= param;
    eng->param(voi_num, Waveform, val);
    voi_dat[voi_num].waveform = val;
    pwDisplay->display(param);
}

void chipsynth::setSync()
{
    bool sy;
    if (syncLed->state())
    {
        sy = false;
        syncLed->off();
    }
    else{
        sy = true;
        syncLed->on();
    }

    eng->param(voi_num, Sync, sy);
    voi_dat[voi_num].sync = sy;
}

void chipsynth::setRing()
{
    bool rm;
    if (ringLed->state())
    {
        rm = false;
        ringLed->off();
    }
    else
    {
        rm = true;
        ringLed->on();
    }

    eng->param(voi_num, Ring_mod, rm);
    voi_dat[voi_num].ring_mod = rm;
}

void chipsynth::setArp()
{
    if (arpLed->state()) {
            ctrl = engine::NONE;
            arpLed->off();
    }
    else {
            ctrl = engine::ARP;
            glissLed->off();
            arpLed->on();
    }
    eng->param(voi_num, Control, ctrl);
    voi_dat[voi_num].control = ctrl;
}

void chipsynth::setGliss()
{
    if (glissLed->state())
    {
        ctrl = engine::NONE;
        glissLed->off();
    }
    else
    {
        ctrl = engine::GLISS;
        arpLed->off();
        glissLed->on();
    }

    eng->param(voi_num, Control, ctrl);
    voi_dat[voi_num].control = ctrl;
}

void chipsynth::setFsDepth(int param)
{
    eng->param(voi_num, Filter_sweep_depth, param);
    fsDepthDisplay->display(param);
    voi_dat[voi_num].fs_depth = param;
}

void chipsynth::setFsSpeed(int param)
{
    eng->param(voi_num, Filter_sweep_speed, param);
    fsSpeedDisplay->display(param);
    voi_dat[voi_num].fs_speed = param;
}

void chipsynth::setSplit(int param)
{
    //eng->split(param);
    splitDisplay->display(param);
}

void chipsynth::setVoice1()
{
    voice1Led->on();
    voice2Led->off();
    voi_num = 0;
    displayParams(0);
}

void chipsynth::setVoice2()
{
    voice2Led->on();
    voice1Led->off();
    voi_num = 1;
    displayParams(1);
}

void chipsynth::setPmSpeed(int param)
{
    int val = voi_dat[voi_num].pm & ~wavegen_module::PW_SPEED_MASK;
    val |= param;
    eng->param(voi_num, PwMod, val);
    pmSpeedDisplay->display(param);
    voi_dat[voi_num].pm = val;
}

void chipsynth::setPmDepth(int param)
{
    int val = voi_dat[voi_num].pm & ~wavegen_module::PW_DEPTH_MASK;
    val |= (param << 4);
    eng->param(voi_num, PwMod, val);
    pmDepthDisplay->display(param);
    voi_dat[voi_num].pm = val;
}

void chipsynth::setVibSpeed(int param)
{
    eng->param(voi_num, Vibrato_speed, param);
    vibSpdDisplay->display(param);
    voi_dat[voi_num].vib_speed = param;
}

void chipsynth::setEchoTime(int param)
{
    int val = voi_dat[voi_num].echo & ~echo::TIME_MASK;
    val |= (param << 8);
    eng->param(voi_num, Echo, val);
    echoTimeDisplay->display(param);
    voi_dat[voi_num].echo = val;
}

void chipsynth::setEchoFbk(int param)
{
    int val = voi_dat[voi_num].echo & ~echo::FBK_MASK;
    val |= (param << 4);
    eng->param(voi_num, Echo, val);
    echoFbkDisplay->display(param);
    voi_dat[voi_num].echo = val;
}

void chipsynth::setEchoMix(int param)
{
    int val = voi_dat[voi_num].echo & ~echo::MIX_MASK;
    val |= param;
    eng->param(voi_num, Echo, val);
    echoMixDisplay->display(param);
    voi_dat[voi_num].echo = val;
}

void chipsynth::setChorusRate(int param)
{
    int val = voi_dat[voi_num].chorus & ~chorus::RATE_MASK;
    val |= (param << 8);
    eng->param(voi_num, Chorus, val);
    chorusRateDisplay->display(param);
    voi_dat[voi_num].chorus = val;
}

void chipsynth::setChorusFbk(int param)
{
    int val = voi_dat[voi_num].chorus & ~chorus::FBK_MASK;
    val |= (param << 4);
    eng->param(voi_num, Chorus, val);
    chorusFbkDisplay->display(param);
    voi_dat[voi_num].chorus = val;
}

void chipsynth::setChorusMix(int param)
{
    int val = voi_dat[voi_num].chorus & ~chorus::MIX_MASK;
    val |= param;
    eng->param(voi_num, Chorus, val);
    chorusMixDisplay->display(param);
    voi_dat[voi_num].chorus = val;
}

void chipsynth::displayParams(int v_num)
{
    uint16_t val;
    volumeDisplay->display(voi_dat[v_num].volume);
    volumeKnob->setValue(voi_dat[v_num].volume);
    filt = voi_dat[v_num].filt_type;
    if (filt & filter::LOWPASS)
        lpLed->on();
    else
        lpLed->off();
    if (filt & filter::BANDPASS)
        bpLed->on();
    else
        bpLed->off();
    if (filt & filter::HIGHPASS)
        hpLed->on();
    else
        hpLed->off();
    cutoffDisplay->display(voi_dat[v_num].filt_cutoff);
    cutoffKnob->setValue(voi_dat[v_num].filt_cutoff);
    resDisplay->display(voi_dat[v_num].filt_res);
    resKnob->setValue(voi_dat[v_num].filt_res);

    val = envelope::ATTACK(voi_dat[v_num].adsr);
    attackDisplay->display(val);
    attackKnob->setValue(val);

    val = envelope::DECAY(voi_dat[v_num].adsr);
    decayDisplay->display(val);
    decayKnob->setValue(val);

    val = envelope::SUSTAIN(voi_dat[v_num].adsr);
    sustainDisplay->display(val);
    sustainKnob->setValue(val);

    val = envelope::RELEASE(voi_dat[v_num].adsr);
    releaseDisplay->display(val);
    releaseKnob->setValue(val);

    val = voi_dat[v_num].waveform;
    if (val & wavegen::TRI)
        triLed->on();
    else
        triLed->off();
    if (val & wavegen::SAW)
        sawLed->on();
    else
        sawLed->off();
    if (val & wavegen::PUL)
        pulLed->on();
    else
        pulLed->off();
    if (val & wavegen::NOI)
        noiLed->on();
    else
        noiLed->off();

    val = wavegen::PULSEWIDTH(val);
    pwDisplay->display(val);
    pwKnob->setValue(val);

    val = echo::TIME(voi_dat[v_num].echo);
    echoTimeDisplay->display(val);
    echoTimeKnob->setValue(val);

    val = echo::FBK(voi_dat[v_num].echo);
    echoFbkDisplay->display(val);
    echoFbkKnob->setValue(val);

    val = echo::MIX(voi_dat[v_num].echo);
    echoMixDisplay->display(val);
    echoMixKnob->setValue(val);

    val = chorus::RATE(voi_dat[v_num].chorus);
    chorusRateDisplay->display(val);
    chorusRateKnob->setValue(val);

    val = chorus::FBK(voi_dat[v_num].chorus);
    chorusFbkDisplay->display(val);
    chorusFbkKnob->setValue(val);

    val = chorus::MIX(voi_dat[v_num].chorus);
    chorusMixDisplay->display(val);
    chorusMixKnob->setValue(val);

    ctrl = voi_dat[v_num].control;
    if (ctrl == engine::GLISS)
        glissLed->on();
    else
        glissLed->off();
    if (ctrl == engine::ARP)
        arpLed->on();
    else
        arpLed->off();
    if (voi_dat[v_num].ring_mod)
        ringLed->on();
    else
        ringLed->off();
    if (voi_dat[v_num].sync)
        syncLed->on();
    else
        syncLed->off();

    fsDepthDisplay->display(voi_dat[voi_num].fs_depth);
    fsDepthKnob->setValue(voi_dat[voi_num].fs_depth);
    fsSpeedDisplay->display(voi_dat[voi_num].fs_speed);
    fsSpeedKnob->setValue(voi_dat[voi_num].fs_speed);

    val = wavegen_module::PW_SPEED(voi_dat[voi_num].pm);
    pmSpeedDisplay->display(val);
    pmSpeedKnob->setValue(val);

    val = wavegen_module::PW_DEPTH(voi_dat[voi_num].pm);
    pmDepthDisplay->display(val);
    pmDepthKnob->setValue(val);

    vibSpdDisplay->display(voi_dat[voi_num].vib_speed);
    vibSpdKnob->setValue(voi_dat[voi_num].vib_speed);
}

void chipsynth::reset()
{
    qWarning("chipsynth::reset(): Not implemented yet");
}

void chipsynth::config()
{
    configDialog cfg(this);
    if (cfg.exec() == QDialog::Accepted)
    {
    }
}

void chipsynth::setFiltAll()
{
    qWarning("chipsynth::setFiltAll(): Not implemented yet");
}

void chipsynth::setFiltEcho()
{
    qWarning("chipsynth::setFiltEcho(): Not implemented yet");
}

void chipsynth::init()
{
    setWindowIcon(QIcon(":csicon"));

    memset(voi_dat, 0, sizeof(voi_dat));

    const unsigned int channels = 1; // FIXME
    const unsigned int samplerate = 48000; // FIXME

    const int cnt = (int)(engine::CLOCK / samplerate);

    eng = new engine();
    aMid = new csMidi(this, eng);
    aAud = new csAudio(cnt, eng);

    aAud->open(0, samplerate, channels); // FIXME
    aMid->open();
}

void chipsynth::destroy()
{
    aMid->close();
    aAud->close();

    delete aAud;
    delete aMid;
    delete eng;
}

