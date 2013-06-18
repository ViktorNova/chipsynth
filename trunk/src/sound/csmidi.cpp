/*
 * This file is part of ChipSynth.
 *
 * Copyright 2013 Leandro Nini
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

#include "csmidi.h"

#include <iostream>
#include <stdexcept>
#include <cstdlib>

#include "RtMidi.h"
#include "RtError.h"
#include "engine.h"

void mycallback(double deltatime, std::vector<unsigned char> *message, void *userData)
{
    engine *eng = static_cast<engine*>(userData);

#ifdef DEBUG
    unsigned int nBytes = message->size();
    for ( unsigned int i=0; i<nBytes; i++ )
        std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
    if ( nBytes > 0 )
        std::cout << "stamp = " << deltatime << std::endl;
#endif

    unsigned char status = 0;
    unsigned char data1 = 0;
    unsigned char data2 = 0;
    try
    {
        status = message->at(0);
        data1 = message->at(1);
        data2 = message->at(2);
    }
    catch (const std::out_of_range &e) {}

    if (status >= 0x80 && status <= 0x8F)
    {
        const int channel = status - 0x80;
        eng->noteOff(channel, data1);
    }
    else if (status >= 0x90 && status <= 0x9F)
    {
        const int channel = status - 0x90;
        // many devices send a 'note on' event with velocity 0
        // rather than a distinct 'note off' event
        if (data2 != 0)
        {
             eng->noteOn(channel, data1);
        }
        else
        {
             eng->noteOff(channel, data1);
        }
    }
    else if (status >= 0xB0 && status <= 0xBF)
    {
        // TODO
#if 0
        switch (data1)
        {
        case 1:         // modulation (vibrato?)
            _cs->setVibSpeed(data2);
            _cs->vibSpdKnob->setValue(data2);
            break;
        case 7:         // volume
            _cs->setVolume(data2 / 8);
            _cs->volumeKnob->setValue(data2 / 8);
            break;
        case 10:        // panning
            break;
        case 11:        // expression
            break;
        case 64:        // sustain
            _cs->setSustain(data2 / 8);
            _cs->sustainKnob->setValue(data2 / 8);
            break;
        case 66:        // sostenuto
            break;
        case 91:        // reverb
            break;
        case 93:        // chorus
            break;
        }
#endif
    }
    else if (status >= 0xE0 && status <= 0xEF)
    {
        const int channel = status - 0xE0;
        int value =  data1 | (data2 << 7);
        eng->param(channel, PitchBend, value);
    }
}

csMidi::csMidi(chipsynth *cs, engine *eng) :
    _cs(cs),
    _eng(eng),
    _midiin(0)
{
    try
    {
        _midiin = new RtMidiIn();
    }
    catch (const RtError &error)
    {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
}

csMidi::~csMidi()
{
    delete _midiin;
}
    
bool csMidi::open()
{
    // Check available ports.
    unsigned int nPorts = _midiin->getPortCount();
    if ( nPorts == 0 ) {
        std::cout << "No ports available!" << std::endl;
        return false;
    }

    _midiin->openPort(0);
    //midiin->openVirtualPort(std::string("RtMidi Input"));

    _midiin->setCallback(&mycallback, _eng);

    return true;
}

void csMidi::close()
{
    _midiin->closePort();
}
