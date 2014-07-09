/*
 * GamepadInputSettings.hpp
 *
 *  Created on: Jun 27, 2014
 *      Author: Travis
 */

#ifndef GAMEPADINPUTSETTINGS_HPP_
#define GAMEPADINPUTSETTINGS_HPP_

#include "SettingsBase.hpp"
#include "InputSettings.hpp"

#include <QObject>
#include <QSettings>


class GamepadInputSettings : public InputSettings
{
    Q_OBJECT

    Q_PROPERTY(QString ControllerID READ ControllerID WRITE ControllerID NOTIFY ControllerIDChanged)
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
    Q_PROPERTY(int AnalogStickType READ AnalogStickType WRITE AnalogStickType NOTIFY AnalogStickTypeChanged)
    Q_PROPERTY(int XAxisLeft READ XAxisLeft WRITE XAxisLeft NOTIFY XAxisLeftChanged)
    Q_PROPERTY(int XAxisRight READ XAxisRight WRITE XAxisRight NOTIFY XAxisRightChanged)
    Q_PROPERTY(int YAxisUp READ YAxisUp WRITE YAxisUp NOTIFY YAxisUpChanged)
    Q_PROPERTY(int YAxisDown READ YAxisDown WRITE YAxisDown NOTIFY YAxisDownChanged)

signals:
    void ControllerIDChanged();
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
    void AnalogStickTypeChanged();
    void XAxisLeftChanged();
    void XAxisRightChanged();
    void YAxisUpChanged();
    void YAxisDownChanged();

private:
    enum DefaultKeys
    {
        DPADR = 1 << 19,
        DPADL = 1 << 18,
        DPADD = 1 << 17,
        DPADU = 1 << 16,
        START = 1 << 11,
        ZTRIGGER = 1 << 14,
        BBUTTON = 1 << 1,
        ABUTTON = 1,
        CBUTTONR = -1,
        CBUTTONL = -1,
        CBUTTOND = -1,
        CBUTTONU = -1,
        RTRIGGER = 1 << 13,
        LTRIGGER = 1 << 10,
        XAXISLEFT = -1,
        XAXISRIGHT = -1,
        YAXISUP = -1,
        YAXISDOWN = -1
    };

public:
    QString ControllerID() { return getStringInput("GAMEPAD_CONTROLLER_ID", ""); }
    void ControllerID(const QString& val) { setStringInput("GAMEPAD_CONTROLLER_ID", val); }

    int DPadR() { return getIntInput("GAMEPAD_DPADR", DPADR); }
    void DPadR(int val) { setIntInput(QString("GAMEPAD_DPADR"), val); }

    int DPadL() { return getIntInput("GAMEPAD_DPADL", DPADL); }
    void DPadL(int val) { setIntInput("GAMEPAD_DPADL", val); }

    int DPadD() { return getIntInput("GAMEPAD_DPADD", DPADD); }
    void DPadD(int val) { setIntInput("GAMEPAD_DPADD", val); }

    int DPadU() { return getIntInput("GAMEPAD_DPADU", DPADU); }
    void DPadU(int val) { setIntInput("GAMEPAD_DPADU", val); }

    int Start() { return getIntInput("GAMEPAD_START", START); }
    void Start(int val) { setIntInput("GAMEPAD_START", val); }

    int ZTrigger() { return getIntInput("GAMEPAD_ZTRIGGER", ZTRIGGER); }
    void ZTrigger(int val) { setIntInput("GAMEPAD_ZTRIGGER", val); }

    int BButton() { return getIntInput("GAMEPAD_BBUTTON", BBUTTON); }
    void BButton(int val) { setIntInput("GAMEPAD_BBUTTON", val); }

    int AButton() { return getIntInput("GAMEPAD_ABUTTON", ABUTTON); }
    void AButton(int val) { setIntInput("GAMEPAD_ABUTTON", val); }

    int CButtonR() { return getIntInput("GAMEPAD_CBUTTONR", CBUTTONR); }
    void CButtonR(int val) { setIntInput("GAMEPAD_CBUTTONR", val); }

    int CButtonL() { return getIntInput("GAMEPAD_CBUTTONL", CBUTTONL); }
    void CButtonL(int val) { setIntInput("GAMEPAD_CBUTTONL", val); }

    int CButtonD() { return getIntInput("GAMEPAD_CBUTTOND", CBUTTOND); }
    void CButtonD(int val) { setIntInput("GAMEPAD_CBUTTOND", val); }

    int CButtonU() { return getIntInput("GAMEPAD_CBUTTONU", CBUTTONU); }
    void CButtonU(int val) { setIntInput("GAMEPAD_CBUTTONU", val); }

    int RTrigger() { return getIntInput("GAMEPAD_RTRIGGER", RTRIGGER); }
    void RTrigger(int val) { setIntInput("GAMEPAD_RTRIGGER", val); }

    int LTrigger() { return getIntInput("GAMEPAD_LTRIGGER", LTRIGGER); }
    void LTrigger(int val) { setIntInput("GAMEPAD_LTRIGGER", val); }

    int AnalogStickType() { return getIntInput("GAMEPAD_ANALOG", 1); }
    void AnalogStickType(int val) { setIntInput("GAMEPAD_ANALOG", val); }

    int XAxisLeft() { return getIntInput("GAMEPAD_XAXISL", XAXISLEFT); }
    void XAxisLeft(int val) { setIntInput("GAMEPAD_XAXISL", val); }

    int XAxisRight() { return getIntInput("GAMEPAD_XAXISR", XAXISRIGHT); }
    void XAxisRight(int val) { setIntInput("GAMEPAD_XAXISR", val); }

    int YAxisDown() { return getIntInput("GAMEPAD_YAXISD", YAXISDOWN); }
    void YAxisDown(int val) { setIntInput("GAMEPAD_YAXISD", val); }

    int YAxisUp() { return getIntInput("GAMEPAD_YAXISU", YAXISUP); }
    void YAxisUp(int val) { setIntInput("GAMEPAD_YAXISU", val); }

public:
    GamepadInputSettings() : InputSettings(1) { }
    GamepadInputSettings(int player) : InputSettings(player) { }
    GamepadInputSettings(const GamepadInputSettings& toCopy) : InputSettings(toCopy) { }

    GamepadInputSettings operator=(const GamepadInputSettings& toCopy) { InputSettings::operator=(toCopy); return *this; }

    Q_INVOKABLE virtual void reset();
    Q_INVOKABLE virtual void clear();
    virtual void writeSettings(Emulator *m64p);

protected:
    virtual void GameNameChanged();
};

#endif /* GAMEPADINPUTSETTINGS_HPP_ */
