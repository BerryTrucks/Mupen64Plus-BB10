/*
 * HardwareSettings.cpp
 *
 *  Created on: Aug 1, 2014
 *      Author: Travis
 */

#include "HardwareSettings.hpp"


void HardwareSettings::writeSettings(Emulator* m64p)
{
    if(ExpansionPak())
        m64p->SetConfigParameter(std::string("Core[DisableExtraMem]=False"));
    else
        m64p->SetConfigParameter(std::string("Core[DisableExtraMem]=True"));

    m64p->SetConfigParameter(std::string("Core[R4300Emulator]=") + QString::number(R4300Emulation()).toStdString());
}


