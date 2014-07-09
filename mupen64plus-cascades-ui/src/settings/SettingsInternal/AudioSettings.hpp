/*
 * AudioSettings.hpp
 *
 *  Created on: Jun 26, 2014
 *      Author: Travis
 */

#ifndef AUDIOSETTINGS_HPP_
#define AUDIOSETTINGS_HPP_

#include "SettingsBase.hpp"

#include <QObject>
#include <QSettings>


class AudioSettings : public SettingsBase
{
    Q_OBJECT

    Q_PROPERTY(bool AudioEnabled READ AudioEnabled WRITE AudioEnabled NOTIFY AudioEnabledChanged)

signals:
    void AudioEnabledChanged();

public:
    bool AudioEnabled() { return getBool("AUDIO_ENABLED", true); }
    void AudioEnabled(bool val) { setBool("AUDIO_ENABLED", val); }

    Q_INVOKABLE virtual void reset();
    virtual void writeSettings(Emulator *m64p);

public:
    AudioSettings() { }
    AudioSettings(const AudioSettings& toCopy) : SettingsBase() { Q_UNUSED(toCopy); }

    AudioSettings operator=(const AudioSettings& toCopy) { Q_UNUSED(toCopy); return *this; }

protected:
    virtual void GameNameChanged();
};

#endif /* AUDIOSETTINGS_HPP_ */
