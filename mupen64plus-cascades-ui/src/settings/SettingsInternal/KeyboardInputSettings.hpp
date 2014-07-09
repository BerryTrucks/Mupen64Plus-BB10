/*
 * KeyboardInputSettings.hpp
 *
 *  Created on: Jun 27, 2014
 *      Author: Travis
 */

#ifndef KEYBOARDINPUTSETTINGS_HPP_
#define KEYBOARDINPUTSETTINGS_HPP_

#include "SettingsBase.hpp"
#include "InputSettings.hpp"

#include <QObject>
#include <QSettings>

#include <sys/keycodes.h>


class KeyboardInputSettings : public InputSettings
{
    Q_OBJECT

    Q_PROPERTY(bool RumblePak READ RumblePak WRITE RumblePak NOTIFY RumblePakChanged)
    Q_PROPERTY(int DPadR READ DPadR WRITE DPadR NOTIFY DPadRChanged)
    Q_PROPERTY(int DPadL READ DPadL WRITE DPadL NOTIFY DPadLChanged)
    Q_PROPERTY(int DPadD READ DPadD WRITE DPadD NOTIFY DPadDChanged)
    Q_PROPERTY(int DPadU READ DPadU WRITE DPadU NOTIFY DPadUChanged)
    Q_PROPERTY(int Start READ Start WRITE Start NOTIFY StartChanged)
    Q_PROPERTY(int ZTrigger READ ZTrigger WRITE ZTrigger NOTIFY ZTriggerChanged)
    Q_PROPERTY(int BButton READ BButton WRITE BButton NOTIFY BButtonChanged)
    Q_PROPERTY(int AButton READ AButton WRITE AButton NOTIFY AButtonChanged)
    Q_PROPERTY(int CButtonR READ CButtonR WRITE CButtonR NOTIFY CButtonRChanged)
    Q_PROPERTY(int CButtonL READ CButtonL WRITE CButtonL NOTIFY CButtonLChanged)
    Q_PROPERTY(int CButtonD READ CButtonD WRITE CButtonD NOTIFY CButtonDChanged)
    Q_PROPERTY(int CButtonU READ CButtonU WRITE CButtonU NOTIFY CButtonUChanged)
    Q_PROPERTY(int RTrigger READ RTrigger WRITE RTrigger NOTIFY RTriggerChanged)
    Q_PROPERTY(int LTrigger READ LTrigger WRITE LTrigger NOTIFY LTriggerChanged)
    Q_PROPERTY(int XAxisLeft READ XAxisLeft WRITE XAxisLeft NOTIFY XAxisLeftChanged)
    Q_PROPERTY(int XAxisRight READ XAxisRight WRITE XAxisRight NOTIFY XAxisRightChanged)
    Q_PROPERTY(int YAxisUp READ YAxisUp WRITE YAxisUp NOTIFY YAxisUpChanged)
    Q_PROPERTY(int YAxisDown READ YAxisDown WRITE YAxisDown NOTIFY YAxisDownChanged)
    Q_PROPERTY(int UpLeft READ UpLeft WRITE UpLeft NOTIFY UpLeftChanged)
    Q_PROPERTY(int UpRight READ UpRight WRITE UpRight NOTIFY UpRightChanged)
    Q_PROPERTY(int DownLeft READ DownLeft WRITE DownLeft NOTIFY DownLeftChanged)
    Q_PROPERTY(int DownRight READ DownRight WRITE DownRight NOTIFY DownRightChanged)

signals:
    void RumblePakChanged();
    void DPadRChanged();
    void DPadLChanged();
    void DPadDChanged();
    void DPadUChanged();
    void StartChanged();
    void ZTriggerChanged();
    void BButtonChanged();
    void AButtonChanged();
    void CButtonRChanged();
    void CButtonLChanged();
    void CButtonDChanged();
    void CButtonUChanged();
    void RTriggerChanged();
    void LTriggerChanged();
    void XAxisLeftChanged();
    void XAxisRightChanged();
    void YAxisUpChanged();
    void YAxisDownChanged();
    void UpLeftChanged();
    void UpRightChanged();
    void DownLeftChanged();
    void DownRightChanged();

protected:
    enum DefaultKeys
    {
        DPADR = KEYCODE_H,
        DPADL = KEYCODE_F,
        DPADD = KEYCODE_G,
        DPADU = KEYCODE_T,
        START = KEYCODE_SPACE,
        ZTRIGGER = KEYCODE_O,
        BBUTTON = KEYCODE_K,
        ABUTTON = KEYCODE_L,
        CBUTTONR = KEYCODE_M,
        CBUTTONL = KEYCODE_B,
        CBUTTOND = KEYCODE_N,
        CBUTTONU = KEYCODE_J,
        RTRIGGER = KEYCODE_P,
        LTRIGGER = KEYCODE_I,
        XAXISLEFT = KEYCODE_A,
        XAXISRIGHT = KEYCODE_D,
        YAXISUP = KEYCODE_W,
        YAXISDOWN = KEYCODE_S,
        UPLEFT = -1,
        UPRIGHT = -1,
        DOWNLEFT = -1,
        DOWNRIGHT = -1
    };

public:
    bool RumblePak() { return getBoolInput("KEYBOARD_RUMBLE", false); }
    void RumblePak(bool val) { setBoolInput("KEYBOARD_RUMBLE", val); }

    int DPadR() { return getIntInput("KEYBOARD_DPADR", DPADR); }
    void DPadR(int val) { setIntInput("KEYBOARD_DPADR", val); }

    int DPadL() { return getIntInput("KEYBOARD_DPADL", DPADL); }
    void DPadL(int val) { setIntInput("KEYBOARD_DPADL", val); }

    int DPadD() { return getIntInput("KEYBOARD_DPADD", DPADD); }
    void DPadD(int val) { setIntInput("KEYBOARD_DPADD", val); }

    int DPadU() { return getIntInput("KEYBOARD_DPADU", DPADU); }
    void DPadU(int val) { setIntInput("KEYBOARD_DPADU", val); }

    int Start() { return getIntInput("KEYBOARD_START", START); }
    void Start(int val) { setIntInput("KEYBOARD_START", val); }

    int ZTrigger() { return getIntInput("KEYBOARD_ZTRIGGER", ZTRIGGER); }
    void ZTrigger(int val) { setIntInput("KEYBOARD_ZTRIGGER", val); }

    int BButton() { return getIntInput("KEYBOARD_BBUTTON", BBUTTON); }
    void BButton(int val) { setIntInput("KEYBOARD_BBUTTON", val); }

    int AButton() { return getIntInput("KEYBOARD_ABUTTON", ABUTTON); }
    void AButton(int val) { setIntInput("KEYBOARD_ABUTTON", val); }

    int CButtonR() { return getIntInput("KEYBOARD_CBUTTONR", CBUTTONR); }
    void CButtonR(int val) { setIntInput("KEYBOARD_CBUTTONR", val); }

    int CButtonL() { return getIntInput("KEYBOARD_CBUTTONL", CBUTTONL); }
    void CButtonL(int val) { setIntInput("KEYBOARD_CBUTTONL", val); }

    int CButtonD() { return getIntInput("KEYBOARD_CBUTTOND", CBUTTOND); }
    void CButtonD(int val) { setIntInput("KEYBOARD_CBUTTOND", val); }

    int CButtonU() { return getIntInput("KEYBOARD_CBUTTONU", CBUTTONU); }
    void CButtonU(int val) { setIntInput("KEYBOARD_CBUTTONU", val); }

    int RTrigger() { return getIntInput("KEYBOARD_RTRIGGER", RTRIGGER); }
    void RTrigger(int val) { setIntInput("KEYBOARD_RTRIGGER", val); }

    int LTrigger() { return getIntInput("KEYBOARD_LTRIGGER", LTRIGGER); }
    void LTrigger(int val) { setIntInput("KEYBOARD_LTRIGGER", val); }

    int XAxisLeft() { return getIntInput("KEYBOARD_XAXISL", XAXISLEFT); }
    void XAxisLeft(int val) { setIntInput("KEYBOARD_XAXISL", val); }

    int XAxisRight() { return getIntInput("KEYBOARD_XAXISR", XAXISRIGHT); }
    void XAxisRight(int val) { setIntInput("KEYBOARD_XAXISR", val); }

    int YAxisDown() { return getIntInput("KEYBOARD_YAXISD", YAXISDOWN); }
    void YAxisDown(int val) { setIntInput("KEYBOARD_YAXISD", val); }

    int YAxisUp() { return getIntInput("KEYBOARD_YAXISU", YAXISUP); }
    void YAxisUp(int val) { setIntInput("KEYBOARD_YAXISU", val); }

    int UpLeft() { return getIntInput("KEYBOARD_UPLEFT", UPLEFT); }
    void UpLeft(int val) { setIntInput("KEYBOARD_UPLEFT", val); }

    int UpRight() { return getIntInput("KEYBOARD_UPRIGHT", UPRIGHT); }
    void UpRight(int val) { setIntInput("KEYBOARD_UPRIGHT", val); }

    int DownLeft() { return getIntInput("KEYBOARD_DOWNLEFT", DOWNLEFT); }
    void DownLeft(int val) { setIntInput("KEYBOARD_DOWNLEFT", val); }

    int DownRight() { return getIntInput("KEYBOARD_DOWNRIGHT", DOWNRIGHT); }
    void DownRight(int val) { setIntInput("KEYBOARD_DOWNRIGHT", val); }

public:
    KeyboardInputSettings() : InputSettings(1) { }
    KeyboardInputSettings(int player) : InputSettings(player) { }
    KeyboardInputSettings(const KeyboardInputSettings& toCopy) : InputSettings(toCopy) { }

    KeyboardInputSettings operator=(const KeyboardInputSettings& toCopy) { InputSettings::operator=(toCopy); return *this; }

    Q_INVOKABLE virtual void reset();
    Q_INVOKABLE virtual void clear();
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged();
};

#endif /* KEYBOARDINPUTSETTINGS_HPP_ */
