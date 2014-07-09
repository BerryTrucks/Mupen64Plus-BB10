/*
 * AudioSettings.cpp
 *
 *  Created on: Jun 26, 2014
 *      Author: Travis
 */

#include "AudioSettings.hpp"


void AudioSettings::GameNameChanged()
{
    emit AudioEnabledChanged();
}

void AudioSettings::reset()
{
    AudioEnabled(true);
    emit AudioEnabledChanged();
}

void AudioSettings::writeSettings(Emulator *m64p)
{
    if(AudioEnabled())
        m64p->SetConfigParameter(std::string("UI-Console[AudioPlugin]=")+"mupen64plus-audio-sdl");
    else
        m64p->SetConfigParameter(std::string("UI-Console[AudioPlugin]=")+"dummy");
}


