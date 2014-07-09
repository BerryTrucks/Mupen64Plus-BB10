/*
 * TouchscreenInputSettings.hpp
 *
 *  Created on: Jun 27, 2014
 *      Author: Travis
 */

#ifndef TOUCHSCREENINPUTSETTINGS_HPP_
#define TOUCHSCREENINPUTSETTINGS_HPP_

#include "SettingsBase.hpp"
#include "InputSettings.hpp"

#include <QObject>
#include <QSettings>


class TouchscreenInputSettings : public InputSettings
{
    Q_OBJECT

    Q_PROPERTY(bool RumblePak READ RumblePak WRITE RumblePak NOTIFY RumblePakChanged)
    Q_PROPERTY(int ControllerLayout READ ControllerLayout WRITE ControllerLayout NOTIFY ControllerLayoutChanged)

signals:
    void RumblePakChanged();
    void ControllerLayoutChanged();

public:
    bool RumblePak() { return getBoolInput("TOUCHSCREEN_RUMBLE", false); }
    void RumblePak(bool val) { setBoolInput("TOUCHSCREEN_RUMBLE", val); }

    int ControllerLayout() { return getIntInput("TOUCHSCREEN_LAYOUT", 1); }
    void ControllerLayout(int val) { setIntInput("TOUCHSCREEN_LAYOUT", val); }

public:
    TouchscreenInputSettings() : InputSettings(1) { }
    TouchscreenInputSettings(int player) : InputSettings(player) { }
    TouchscreenInputSettings(const TouchscreenInputSettings& toCopy) : InputSettings(toCopy) { }

    TouchscreenInputSettings operator=(const TouchscreenInputSettings& toCopy) { InputSettings::operator=(toCopy); return *this; }

    Q_INVOKABLE virtual void reset();
    Q_INVOKABLE virtual void clear();
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged();
};

#endif /* TOUCHSCREENINPUTSETTINGS_HPP_ */
