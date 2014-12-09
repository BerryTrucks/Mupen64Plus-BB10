/*
 * HardwareSettings.hpp
 *
 *  Created on: Aug 1, 2014
 *      Author: Travis
 */

#ifndef HARDWARESETTINGS_HPP_
#define HARDWARESETTINGS_HPP_

#include "SettingsBase.hpp"

#include <stdio.h>

#include <QObject>
#include <QSettings>


class HardwareSettings : public SettingsBase
{
    Q_OBJECT

    Q_PROPERTY(int ExpansionPak READ ExpansionPak WRITE ExpansionPak NOTIFY ExpansionPakChanged)
    Q_PROPERTY(int R4300Emulation READ R4300Emulation WRITE R4300Emulation NOTIFY R4300EmulationChanged)

signals:
    void ExpansionPakChanged();
    void R4300EmulationChanged();

public:
    bool ExpansionPak() { return getBool("HARDWARE_EXPANSION", true); }
    void ExpansionPak(bool val) { setBool("HARDWARE_EXPANSION", val); }
    int R4300Emulation() { return getInt("HARDWARE_EMULATION", 2); }
    void R4300Emulation(int val) { setInt("HARDWARE_EMULATION", val); }

public:
    HardwareSettings() { }
    HardwareSettings(const HardwareSettings& toCopy) : SettingsBase() { Q_UNUSED(toCopy); }

    HardwareSettings operator=(const HardwareSettings& toCopy) { Q_UNUSED(toCopy); return *this; }

    Q_INVOKABLE virtual void reset() { ExpansionPak(true); emit ExpansionPakChanged(); R4300Emulation(2); emit R4300EmulationChanged(); }
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged() { emit ExpansionPakChanged(); }
};

#endif /* HARDWARESETTINGS_HPP_ */
