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

#include "configDialog.h"

#include <vector>
#include <iterator>

Q_DECLARE_METATYPE(RtAudio::Api)

const char* getApiName(RtAudio::Api api)
{
    switch (api)
    {
    case RtAudio::LINUX_ALSA: return "ALSA";
    case RtAudio::LINUX_PULSE: return "PulseAudio";
    case RtAudio::LINUX_OSS: return "OSS";
    case RtAudio::UNIX_JACK: return "JACK";
    case RtAudio::MACOSX_CORE: return "CORE";
    case RtAudio::WINDOWS_DS: return "DirectSound";
    }
}

void configDialog::fillSoundcardBox()
{
    SoundcardBox->clear();

    RtAudio::Api api=ApiBox->itemData(ApiBox->currentIndex()).value<RtAudio::Api>();
    RtAudio audio(api);
    const unsigned int devices=audio.getDeviceCount();
    for (unsigned int i=0; i<devices; i++)
    {
        RtAudio::DeviceInfo info=audio.getDeviceInfo(i);
        // Check if it supports float32 output
        if (info.probed
                && (info.nativeFormats & RTAUDIO_FLOAT32)
                && (info.outputChannels > 0))
            SoundcardBox->addItem(QString::fromStdString(info.name));
            // TODO: maybe we should cache supported samplerates here
    }
}

void configDialog::fillSamplerateBox()
{
    SamplerateBox->clear();

    RtAudio::Api api=ApiBox->itemData(ApiBox->currentIndex()).value<RtAudio::Api>();
    RtAudio audio(api);
    const int device=SoundcardBox->currentIndex();
    if (device < 0 )
        return;

    RtAudio::DeviceInfo info=audio.getDeviceInfo(device);
    for (std::vector<unsigned int>::iterator it=info.sampleRates.begin(); it!=info.sampleRates.end(); ++it)
    {
        SamplerateBox->addItem(QString::number(*it));
    }
}

configDialog::configDialog(QWidget* parent, Qt::WindowFlags fl) :
    QDialog(parent, fl)
{
    setupUi(this);

    init();
}

configDialog::~configDialog()
{
    destroy();
}

void configDialog::init()
{
    std::vector<RtAudio::Api> apis;
    try
    {
        RtAudio::getCompiledApi(apis);
    }
    catch(const RtError &e)
    {
        e.printMessage();
        return;
    }

    for (std::vector<RtAudio::Api>::iterator it=apis.begin(); it!=apis.end(); ++it)
    {
        ApiBox->addItem(getApiName(*it), QVariant::fromValue(*it));
    }

    fillSoundcardBox();
}

void configDialog::destroy() {}
