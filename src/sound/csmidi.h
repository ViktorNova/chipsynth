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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef CSMIDI_H
#define CSMIDI_H

class chipsynth;
class engine;
class RtMidiIn;

class csMidi
{
public:
   struct userData_t
   {
        engine *eng;
        chipsynth *cs;

        userData_t(chipsynth *c, engine *e) :
            eng(e),
            cs(c) {}
   };

private:
    chipsynth *_cs;
    engine *_eng;
    RtMidiIn *_midiin;
    userData_t _userData;

public:
    csMidi(chipsynth *cs, engine *eng);
    ~csMidi();

    bool open();
    void close();
};

#endif
