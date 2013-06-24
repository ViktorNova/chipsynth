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

#ifndef CONFIGDIALOG_H
#define CONFIGDIALOG_H

#include "configDialog_ui.h"

#include "RtAudio.h"

class configDialog : public QDialog, public Ui::configDialog
{
    Q_OBJECT

public:
    configDialog(QWidget* parent = 0, Qt::WindowFlags fl = 0);
    ~configDialog();

private:
    void init();
    void destroy();

private slots:
    /**
     * Fill the soundcard selection combobox
     * depending on the selectd API
     */
    void fillSoundcardBox();

    /**
     * Fill the samplerate selection combobox
     * depending on the selectd soundcard
     */
    void fillSamplerateBox();
};

#endif // CONFIGDIALOG_H
