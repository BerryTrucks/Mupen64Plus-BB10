/*
 * TouchKeyboardInputSettings.hpp
 *
 *  Created on: Jun 27, 2014
 *      Author: Travis
 */

#ifndef TOUCHKEYBOARDINPUTSETTINGS_HPP_
#define TOUCHKEYBOARDINPUTSETTINGS_HPP_

#include "SettingsBase.hpp"
#include "InputSettings.hpp"

#include <QObject>
#include <QSettings>

#include <sys/keycodes.h>


class TouchKeyboardInputSettings : public InputSettings
{
    Q_OBJECT

    Q_PROPERTY(int RotationDirection READ RotationDirection WRITE RotationDirection NOTIFY RotationDirectionChanged)
    Q_PROPERTY(bool RumblePak READ RumblePak WRITE RumblePak NOTIFY RumblePakChanged)
    Q_PROPERTY(int DPadR READ DPadR WRITE DPadR NOTIFY DPadRChanged)
    Q_PROPERTY(int DPadL READ DPadL WRITE DPadL NOTIFY DPadLChanged)
    Q_PROPERTY(int DPadD READ DPadD WRITE DPadD NOTIFY DPadDChanged)
    Q_PROPERTY(int DPadU READ DPadU WRITE DPadU NOTIFY DPadUChanged)
    Q_PROPERTY(int ZTrigger READ ZTrigger WRITE ZTrigger NOTIFY ZTriggerChanged)
    Q_PROPERTY(int BButton READ BButton WRITE BButton NOTIFY BButtonChanged)
    Q_PROPERTY(int AButton READ AButton WRITE AButton NOTIFY AButtonChanged)
    Q_PROPERTY(int CButtonR READ CButtonR WRITE CButtonR NOTIFY CButtonRChanged)
    Q_PROPERTY(int CButtonL READ CButtonL WRITE CButtonL NOTIFY CButtonLChanged)
    Q_PROPERTY(int CButtonD READ CButtonD WRITE CButtonD NOTIFY CButtonDChanged)
    Q_PROPERTY(int CButtonU READ CButtonU WRITE CButtonU NOTIFY CButtonUChanged)
    Q_PROPERTY(int RTrigger READ RTrigger WRITE RTrigger NOTIFY RTriggerChanged)
    Q_PROPERTY(int LTrigger READ LTrigger WRITE LTrigger NOTIFY LTriggerChanged)

signals:
    void RotationDirectionChanged();
    void RumblePakChanged();
    void DPadRChanged();
    void DPadLChanged();
    void DPadDChanged();
    void DPadUChanged();
    void ZTriggerChanged();
    void BButtonChanged();
    void AButtonChanged();
    void CButtonRChanged();
    void CButtonLChanged();
    void CButtonDChanged();
    void CButtonUChanged();
    void RTriggerChanged();
    void LTriggerChanged();

private:
    enum DefaultKeys
    {
        DPADR = KEYCODE_Z,
        DPADL = KEYCODE_W,
        DPADD = KEYCODE_S,
        DPADU = KEYCODE_D,
        ZTRIGGER = KEYCODE_G,
        BBUTTON = KEYCODE_F,
        ABUTTON = KEYCODE_C,
        CBUTTONR = KEYCODE_B,
        CBUTTONL = KEYCODE_Y,
        CBUTTOND = KEYCODE_H,
        CBUTTONU = KEYCODE_J,
        RTRIGGER = KEYCODE_V,
        LTRIGGER = KEYCODE_T
    };

public:
    int RotationDirection() { return getIntInput("TOUCHKEYBOARD_ROTATION", 0); }
    void RotationDirection(int val) { setIntInput("TOUCHKEYBOARD_ROTATION", val); }

    bool RumblePak() { return getBoolInput("TOUCHKEYBOARD_RUMBLE", false); }
    void RumblePak(bool val) { setBoolInput("TOUCHKEYBOARD_RUMBLE", val); }

    int DPadR() { return getIntInput("TOUCHKEYBOARD_DPADR", DPADR); }
    void DPadR(int val) { setIntInput("TOUCHKEYBOARD_DPADR", val); }

    int DPadL() { return getIntInput("TOUCHKEYBOARD_DPADL", DPADL); }
    void DPadL(int val) { setIntInput("TOUCHKEYBOARD_DPADL", val); }

    int DPadD() { return getIntInput("TOUCHKEYBOARD_DPADD", DPADD); }
    void DPadD(int val) { setIntInput("TOUCHKEYBOARD_DPADD", val); }

    int DPadU() { return getIntInput("TOUCHKEYBOARD_DPADU", DPADU); }
    void DPadU(int val) { setIntInput("TOUCHKEYBOARD_DPADU", val); }

    int ZTrigger() { return getIntInput("TOUCHKEYBOARD_ZTRIGGER", ZTRIGGER); }
    void ZTrigger(int val) { setIntInput("TOUCHKEYBOARD_ZTRIGGER", val); }

    int BButton() { return getIntInput("TOUCHKEYBOARD_BBUTTON", BBUTTON); }
    void BButton(int val) { setIntInput("TOUCHKEYBOARD_BBUTTON", val); }

    int AButton() { return getIntInput("TOUCHKEYBOARD_ABUTTON", ABUTTON); }
    void AButton(int val) { setIntInput("TOUCHKEYBOARD_ABUTTON", val); }

    int CButtonR() { return getIntInput("TOUCHKEYBOARD_CBUTTONR", CBUTTONR); }
    void CButtonR(int val) { setIntInput("TOUCHKEYBOARD_CBUTTONR", val); }

    int CButtonL() { return getIntInput("TOUCHKEYBOARD_CBUTTONL", CBUTTONL); }
    void CButtonL(int val) { setIntInput("TOUCHKEYBOARD_CBUTTONL", val); }

    int CButtonD() { return getIntInput("TOUCHKEYBOARD_CBUTTOND", CBUTTOND); }
    void CButtonD(int val) { setIntInput("TOUCHKEYBOARD_CBUTTOND", val); }

    int CButtonU() { return getIntInput("TOUCHKEYBOARD_CBUTTONU", CBUTTONU); }
    void CButtonU(int val) { setIntInput("TOUCHKEYBOARD_CBUTTONU", val); }

    int RTrigger() { return getIntInput("TOUCHKEYBOARD_RTRIGGER", RTRIGGER); }
    void RTrigger(int val) { setIntInput("TOUCHKEYBOARD_RTRIGGER", val); }

    int LTrigger() { return getIntInput("TOUCHKEYBOARD_LTRIGGER", LTRIGGER); }
    void LTrigger(int val) { setIntInput("TOUCHKEYBOARD_LTRIGGER", val); }

public:
    TouchKeyboardInputSettings() : InputSettings(1) { }
    TouchKeyboardInputSettings(int player) : InputSettings(player) { }
    TouchKeyboardInputSettings(const TouchKeyboardInputSettings& toCopy) : InputSettings(toCopy) { }

    TouchKeyboardInputSettings operator=(const TouchKeyboardInputSettings& toCopy) { InputSettings::operator=(toCopy); return *this; }

    Q_INVOKABLE virtual void reset();
    Q_INVOKABLE virtual void clear();
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged();
};

#endif /* TOUCHKEYBOARDINPUTSETTINGS_HPP_ */
