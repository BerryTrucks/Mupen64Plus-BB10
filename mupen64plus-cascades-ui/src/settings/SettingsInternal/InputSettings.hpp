/*
 * InputSettings.hpp
 *
 *  Created on: Jun 27, 2014
 *      Author: Travis
 */

#ifndef INPUTSETTINGS_HPP_
#define INPUTSETTINGS_HPP_

#include "SettingsBase.hpp"

#include <QObject>
#include <QSettings>


class InputSettings : public SettingsBase
{
    Q_OBJECT

    Q_PROPERTY(int Input READ Input WRITE Input NOTIFY InputChanged)
    Q_PROPERTY(bool ControllerPak READ ControllerPak WRITE ControllerPak NOTIFY ControllerPakChanged)

signals:
    void InputChanged();
    void ControllerPakChanged();

protected:
    virtual bool getBoolInput(const QString &name, bool def) { QString nametemp = m_playerName + name; return getBool(nametemp, def); }
    virtual void setBoolInput(const QString &name, bool val) { QString nametemp = m_playerName + name; setBool(nametemp, val); }

    virtual int getIntInput(const QString &name, int def) { QString nametemp = m_playerName + name; return getInt(nametemp, def); }
    virtual void setIntInput(const QString &name, int val) { QString nametemp = m_playerName + name; setInt(nametemp, val); }

    virtual QString getStringInput(const QString &name, const QString &def) { QString nametemp = m_playerName + name; return getString(nametemp, def); }
    virtual void setStringInput(const QString &name, const QString &val) { QString nametemp = m_playerName + name; setString(nametemp, val); }

    virtual void clearValInput(const QString &name) { QString nametemp = m_playerName + name; clearVal(name); }

public:
    int Input() { return getIntInput("INPUT_INPUT", 0); }
    void Input(int val) { setIntInput("INPUT_INPUT", val); }

    bool ControllerPak() { return getBoolInput("INPUT_CONTROLLERPAK", false); }
    void ControllerPak(bool val) { setBoolInput("INPUT_CONTROLLERPAK", val); }

public:
    InputSettings(int player) { m_player = player; m_playerName = "PLAYER" + QString::number(player & 0b111) + "_"; }
    InputSettings(const InputSettings& toCopy) : SettingsBase() { m_player = toCopy.m_player; m_playerName = QString(toCopy.m_playerName); }

    InputSettings operator=(const InputSettings& toCopy) { m_player = toCopy.m_player; m_playerName = QString(toCopy.m_playerName); return *this; }

    Q_INVOKABLE virtual void reset() { if (m_gameName.isEmpty()) ControllerPak(false); else clearValInput("INPUT_CONTROLLERPAK"); emit ControllerPakChanged(); }
    Q_INVOKABLE virtual void clear() { ControllerPak(false); emit ControllerPakChanged(); }
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged();

protected:
    QString m_playerName;
    int m_player;
};

#endif /* INPUTSETTINGS_HPP_ */
