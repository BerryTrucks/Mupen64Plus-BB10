/*
 * InputSettings.cppv
 *
 *  Created on: Jun 27, 2014
 *      Author: Travis
 */

#include "SettingsBase.hpp"
#include "InputSettings.hpp"
#include "KeyboardInputSettings.hpp"
#include "TouchscreenInputSettings.hpp"
#include "TouchKeyboardInputSettings.hpp"
#include "GamepadInputSettings.hpp"

#include <QStringList>
#include <stdio.h>


void InputSettings::GameNameChanged()
{
    emit InputChanged();
}

void InputSettings::writeSettings(Emulator *m64p)
{
    if (Input() == 0)
        m64p->controller[m_player - 1].present = 0;
    else
    {
        m64p->controller[m_player - 1].present = 1;
        m64p->controller[m_player - 1].device = Input();
    }
}

void KeyboardInputSettings::GameNameChanged()
{
    InputSettings::GameNameChanged();
    emit RumblePakChanged();
    emit DPadRChanged();
    emit DPadLChanged();
    emit DPadDChanged();
    emit DPadUChanged();
    emit StartChanged();
    emit ZTriggerChanged();
    emit BButtonChanged();
    emit AButtonChanged();
    emit CButtonRChanged();
    emit CButtonLChanged();
    emit CButtonDChanged();
    emit CButtonUChanged();
    emit RTriggerChanged();
    emit LTriggerChanged();
    emit XAxisLeftChanged();
    emit XAxisRightChanged();
    emit YAxisUpChanged();
    emit YAxisDownChanged();
    emit UpLeftChanged();
    emit UpRightChanged();
    emit DownLeftChanged();
    emit DownRightChanged();
}

void KeyboardInputSettings::clear()
{
    InputSettings::reset();
    RumblePak(false);
    emit RumblePakChanged();
    DPadR(-1);
    emit DPadRChanged();
    DPadL(-1);
    emit DPadLChanged();
    DPadD(-1);
    emit DPadDChanged();
    DPadU(-1);
    emit DPadUChanged();
    Start(-1);
    emit StartChanged();
    ZTrigger(-1);
    emit ZTriggerChanged();
    BButton(-1);
    emit BButtonChanged();
    AButton(-1);
    emit AButtonChanged();
    CButtonR(-1);
    emit CButtonRChanged();
    CButtonL(-1);
    emit CButtonLChanged();
    CButtonD(-1);
    emit CButtonDChanged();
    CButtonU(-1);
    emit CButtonUChanged();
    RTrigger(-1);
    emit RTriggerChanged();
    LTrigger(-1);
    emit LTriggerChanged();
    XAxisLeft(-1);
    emit XAxisLeftChanged();
    XAxisRight(-1);
    emit XAxisRightChanged();
    YAxisUp(-1);
    emit YAxisUpChanged();
    YAxisDown(-1);
    emit YAxisDownChanged();
    UpLeft(-1);
    emit UpLeftChanged();
    UpRight(-1);
    emit UpRightChanged();
    DownLeft(-1);
    emit DownLeftChanged();
    DownRight(-1);
    emit DownRightChanged();
}

void KeyboardInputSettings::reset()
{
    InputSettings::reset();
    if (m_gameName.isEmpty())
    {
        RumblePak(false);
        DPadR(DPADR);
        DPadL(DPADL);
        DPadD(DPADD);
        DPadU(DPADU);
        Start(START);
        ZTrigger(ZTRIGGER);
        BButton(BBUTTON);
        AButton(ABUTTON);
        CButtonR(CBUTTONR);
        CButtonL(CBUTTONL);
        CButtonD(CBUTTOND);
        CButtonU(CBUTTONU);
        RTrigger(RTRIGGER);
        LTrigger(LTRIGGER);
        XAxisLeft(XAXISLEFT);
        XAxisRight(XAXISRIGHT);
        YAxisUp(YAXISUP);
        YAxisDown(YAXISDOWN);
        UpLeft(UPLEFT);
        UpRight(UPRIGHT);
        DownLeft(DOWNLEFT);
        DownRight(DOWNRIGHT);
    }
    else
    {
        clearValInput("KEYBOARD_DPADR");
        clearValInput("KEYBOARD_DPADL");
        clearValInput("KEYBOARD_DPADD");
        clearValInput("KEYBOARD_DPADU");
        clearValInput("KEYBOARD_START");
        clearValInput("KEYBOARD_ZTRIGGER");
        clearValInput("KEYBOARD_BBUTTON");
        clearValInput("KEYBOARD_ABUTTON");
        clearValInput("KEYBOARD_CBUTTONR");
        clearValInput("KEYBOARD_CBUTTONL");
        clearValInput("KEYBOARD_CBUTTOND");
        clearValInput("KEYBOARD_CBUTTONU");
        clearValInput("KEYBOARD_RTRIGGER");
        clearValInput("KEYBOARD_LTRIGGER");
        clearValInput("KEYBOARD_XAXISL");
        clearValInput("KEYBOARD_XAXISR");
        clearValInput("KEYBOARD_YAXISD");
        clearValInput("KEYBOARD_YAXISU");
        clearValInput("KEYBOARD_UPLEFT");
        clearValInput("KEYBOARD_UPRIGHT");
        clearValInput("KEYBOARD_DOWNLEFT");
        clearValInput("KEYBOARD_DOWNRIGHT");
    }
    emit RumblePakChanged();
    emit DPadRChanged();
    emit DPadLChanged();
    emit DPadDChanged();
    emit DPadUChanged();
    emit StartChanged();
    emit ZTriggerChanged();
    emit BButtonChanged();
    emit AButtonChanged();
    emit CButtonRChanged();
    emit CButtonLChanged();
    emit CButtonDChanged();
    emit CButtonUChanged();
    emit RTriggerChanged();
    emit LTriggerChanged();
    emit XAxisLeftChanged();
    emit XAxisRightChanged();
    emit YAxisUpChanged();
    emit YAxisDownChanged();
    emit UpLeftChanged();
    emit UpRightChanged();
    emit DownLeftChanged();
    emit DownRightChanged();
}

void KeyboardInputSettings::writeSettings(Emulator *m64p)
{
    InputSettings::writeSettings(m64p);
    if (Input() != -2)
        return;
    if (RumblePak())
        m64p->controller[m_player - 1].plugin = 5;
    else
        m64p->controller[m_player - 1].plugin = 0;
    m64p->controller[m_player - 1].button[0] = DPadR();
    m64p->controller[m_player - 1].button[1] = DPadL();
    m64p->controller[m_player - 1].button[2] = DPadD();
    m64p->controller[m_player - 1].button[3] = DPadU();
    m64p->controller[m_player - 1].button[4] = Start();
    m64p->controller[m_player - 1].button[5] = ZTrigger();
    m64p->controller[m_player - 1].button[6] = BButton();
    m64p->controller[m_player - 1].button[7] = AButton();
    m64p->controller[m_player - 1].button[8] = CButtonR();
    m64p->controller[m_player - 1].button[9] = CButtonL();
    m64p->controller[m_player - 1].button[10] = CButtonD();
    m64p->controller[m_player - 1].button[11] = CButtonU();
    m64p->controller[m_player - 1].button[12] = RTrigger();
    m64p->controller[m_player - 1].button[13] = LTrigger();
    m64p->controller[m_player - 1].axis[0].a = XAxisLeft();
    m64p->controller[m_player - 1].axis[0].b = XAxisRight();
    m64p->controller[m_player - 1].axis[1].a = YAxisUp();
    m64p->controller[m_player - 1].axis[1].b = YAxisDown();
    m64p->controller[m_player - 1].diagonals[0] = UpLeft();
    m64p->controller[m_player - 1].diagonals[1] = UpRight();
    m64p->controller[m_player - 1].diagonals[2] = DownLeft();
    m64p->controller[m_player - 1].diagonals[3] = DownRight();
}

void TouchscreenInputSettings::GameNameChanged()
{
    InputSettings::GameNameChanged();
    emit RumblePakChanged();
    emit ControllerLayoutChanged();
}

void TouchscreenInputSettings::clear()
{
    InputSettings::reset();
    RumblePak(false);
    emit RumblePakChanged();
    ControllerLayout(0);
    emit ControllerLayoutChanged();
}

void TouchscreenInputSettings::reset()
{
    InputSettings::reset();
    if (m_gameName.isEmpty())
    {
        RumblePak(false);
        ControllerLayout(1);
    }
    else
    {
        clearValInput("TOUCHSCREEN_RUMBLE");
        clearValInput("TOUCHSCREEN_LAYOUT");
    }
    emit RumblePakChanged();
    emit ControllerLayoutChanged();
}

void TouchscreenInputSettings::writeSettings(Emulator *m64p)
{
    InputSettings::writeSettings(m64p);
    if (Input() != -3)
        return;
    if (RumblePak())
        m64p->controller[m_player - 1].plugin = 5;
    else
        m64p->controller[m_player - 1].plugin = 0;
    m64p->controller[m_player - 1].layout = ControllerLayout();
}

void TouchKeyboardInputSettings::GameNameChanged()
{
    InputSettings::GameNameChanged();
    emit RotationDirectionChanged();
    emit RumblePakChanged();
    emit DPadRChanged();
    emit DPadLChanged();
    emit DPadDChanged();
    emit DPadUChanged();
    emit ZTriggerChanged();
    emit BButtonChanged();
    emit AButtonChanged();
    emit CButtonRChanged();
    emit CButtonLChanged();
    emit CButtonDChanged();
    emit CButtonUChanged();
    emit RTriggerChanged();
    emit LTriggerChanged();
}

void TouchKeyboardInputSettings::clear()
{
    InputSettings::reset();
    RotationDirection(0);
    emit RotationDirectionChanged();
    RumblePak(false);
    emit RumblePakChanged();
    DPadR(-1);
    emit DPadRChanged();
    DPadL(-1);
    emit DPadLChanged();
    DPadD(-1);
    emit DPadDChanged();
    DPadU(-1);
    emit DPadUChanged();
    ZTrigger(-1);
    emit ZTriggerChanged();
    BButton(-1);
    emit BButtonChanged();
    AButton(-1);
    emit AButtonChanged();
    CButtonR(-1);
    emit CButtonRChanged();
    CButtonL(-1);
    emit CButtonLChanged();
    CButtonD(-1);
    emit CButtonDChanged();
    CButtonU(-1);
    emit CButtonUChanged();
    RTrigger(-1);
    emit RTriggerChanged();
    LTrigger(-1);
    emit LTriggerChanged();
}

void TouchKeyboardInputSettings::reset()
{
    InputSettings::reset();
    if (m_gameName.isEmpty())
    {
        RotationDirection(0);
        RumblePak(false);
        DPadR(DPADR);
        DPadL(DPADL);
        DPadD(DPADD);
        DPadU(DPADU);
        ZTrigger(ZTRIGGER);
        BButton(BBUTTON);
        AButton(ABUTTON);
        CButtonR(CBUTTONR);
        CButtonL(CBUTTONL);
        CButtonD(CBUTTOND);
        CButtonU(CBUTTONU);
        RTrigger(RTRIGGER);
        LTrigger(LTRIGGER);
    }
    else
    {
        clearValInput("TOUCHKEYBOARD_ROTATION");
        clearValInput("TOUCHKEYBOARD_RUMBLE");
        clearValInput("TOUCHKEYBOARD_DPADR");
        clearValInput("TOUCHKEYBOARD_DPADL");
        clearValInput("TOUCHKEYBOARD_DPADD");
        clearValInput("TOUCHKEYBOARD_DPADU");
        clearValInput("TOUCHKEYBOARD_ZTRIGGER");
        clearValInput("TOUCHKEYBOARD_BBUTTON");
        clearValInput("TOUCHKEYBOARD_ABUTTON");
        clearValInput("TOUCHKEYBOARD_CBUTTONR");
        clearValInput("TOUCHKEYBOARD_CBUTTONL");
        clearValInput("TOUCHKEYBOARD_CBUTTOND");
        clearValInput("TOUCHKEYBOARD_CBUTTONU");
        clearValInput("TOUCHKEYBOARD_RTRIGGER");
        clearValInput("TOUCHKEYBOARD_LTRIGGER");
    }
    emit RotationDirectionChanged();
    emit RumblePakChanged();
    emit DPadRChanged();
    emit DPadLChanged();
    emit DPadDChanged();
    emit DPadUChanged();
    emit ZTriggerChanged();
    emit BButtonChanged();
    emit AButtonChanged();
    emit CButtonRChanged();
    emit CButtonLChanged();
    emit CButtonDChanged();
    emit CButtonUChanged();
    emit RTriggerChanged();
    emit LTriggerChanged();
}

void TouchKeyboardInputSettings::writeSettings(Emulator *m64p)
{
    InputSettings::writeSettings(m64p);
    if (Input() != -5)
        return;
    if (RumblePak())
        m64p->controller[m_player - 1].plugin = 5;
    else
        m64p->controller[m_player - 1].plugin = 0;
    m64p->controller[m_player - 1].button[0] = DPadR();
    m64p->controller[m_player - 1].button[1] = DPadL();
    m64p->controller[m_player - 1].button[2] = DPadD();
    m64p->controller[m_player - 1].button[3] = DPadU();
    m64p->controller[m_player - 1].button[5] = ZTrigger();
    m64p->controller[m_player - 1].button[6] = BButton();
    m64p->controller[m_player - 1].button[7] = AButton();
    m64p->controller[m_player - 1].button[8] = CButtonR();
    m64p->controller[m_player - 1].button[9] = CButtonL();
    m64p->controller[m_player - 1].button[10] = CButtonD();
    m64p->controller[m_player - 1].button[11] = CButtonU();
    m64p->controller[m_player - 1].button[12] = RTrigger();
    m64p->controller[m_player - 1].button[13] = LTrigger();
}

void GamepadInputSettings::GameNameChanged()
{
    InputSettings::GameNameChanged();
    emit ControllerIDChanged();
    emit DPadRChanged();
    emit DPadLChanged();
    emit DPadDChanged();
    emit DPadUChanged();
    emit StartChanged();
    emit ZTriggerChanged();
    emit BButtonChanged();
    emit AButtonChanged();
    emit CButtonRChanged();
    emit CButtonLChanged();
    emit CButtonDChanged();
    emit CButtonUChanged();
    emit RTriggerChanged();
    emit LTriggerChanged();
    emit AnalogStickTypeChanged();
    emit XAxisLeftChanged();
    emit XAxisRightChanged();
    emit YAxisUpChanged();
    emit YAxisDownChanged();
}

void GamepadInputSettings::clear()
{
    InputSettings::GameNameChanged();
    ControllerID(0);
    emit ControllerIDChanged();
    DPadR(-1);
    emit DPadRChanged();
    DPadL(-1);
    emit DPadLChanged();
    DPadD(-1);
    emit DPadDChanged();
    DPadU(-1);
    emit DPadUChanged();
    Start(-1);
    emit StartChanged();
    ZTrigger(-1);
    emit ZTriggerChanged();
    BButton(-1);
    emit BButtonChanged();
    AButton(-1);
    emit AButtonChanged();
    CButtonR(-1);
    emit CButtonRChanged();
    CButtonL(-1);
    emit CButtonLChanged();
    CButtonD(-1);
    emit CButtonDChanged();
    CButtonU(-1);
    emit CButtonUChanged();
    RTrigger(-1);
    emit RTriggerChanged();
    LTrigger(-1);
    emit LTriggerChanged();
    XAxisLeft(-1);
    emit XAxisLeftChanged();
    XAxisRight(-1);
    emit XAxisRightChanged();
    YAxisUp(-1);
    emit YAxisUpChanged();
    YAxisDown(-1);
    emit YAxisDownChanged();
}

void GamepadInputSettings::reset()
{
    InputSettings::GameNameChanged();
    if (m_gameName.isEmpty())
    {
        ControllerID("");
        DPadR(DPADR);
        DPadL(DPADL);
        DPadD(DPADD);
        DPadU(DPADU);
        Start(START);
        ZTrigger(ZTRIGGER);
        BButton(BBUTTON);
        AButton(ABUTTON);
        CButtonR(CBUTTONR);
        CButtonL(CBUTTONL);
        CButtonD(CBUTTOND);
        CButtonU(CBUTTONU);
        RTrigger(RTRIGGER);
        LTrigger(LTRIGGER);
        AnalogStickType(1);
        XAxisLeft(XAXISLEFT);
        XAxisRight(XAXISRIGHT);
        YAxisUp(YAXISUP);
        YAxisDown(YAXISDOWN);
    }
    else
    {
        clearValInput("GAMEPAD_CONTROLLER_ID");
        clearValInput("GAMEPAD_DPADR");
        clearValInput("GAMEPAD_DPADL");
        clearValInput("GAMEPAD_DPADD");
        clearValInput("GAMEPAD_DPADU");
        clearValInput("GAMEPAD_START");
        clearValInput("GAMEPAD_ZTRIGGER");
        clearValInput("GAMEPAD_BBUTTON");
        clearValInput("GAMEPAD_ABUTTON");
        clearValInput("GAMEPAD_CBUTTONR");
        clearValInput("GAMEPAD_CBUTTONL");
        clearValInput("GAMEPAD_CBUTTOND");
        clearValInput("GAMEPAD_CBUTTONU");
        clearValInput("GAMEPAD_RTRIGGER");
        clearValInput("GAMEPAD_LTRIGGER");
        clearValInput("GAMEPAD_ANALOG");
        clearValInput("GAMEPAD_XAXISL");
        clearValInput("GAMEPAD_XAXISR");
        clearValInput("GAMEPAD_YAXISD");
        clearValInput("GAMEPAD_YAXISU");
    }
    emit ControllerIDChanged();
    emit DPadRChanged();
    emit DPadLChanged();
    emit DPadDChanged();
    emit DPadUChanged();
    emit StartChanged();
    emit ZTriggerChanged();
    emit BButtonChanged();
    emit AButtonChanged();
    emit CButtonRChanged();
    emit CButtonLChanged();
    emit CButtonDChanged();
    emit CButtonUChanged();
    emit RTriggerChanged();
    emit LTriggerChanged();
    emit XAxisLeftChanged();
    emit XAxisRightChanged();
    emit YAxisUpChanged();
    emit YAxisDownChanged();
}

void GamepadInputSettings::writeSettings(Emulator *m64p)
{
    InputSettings::writeSettings(m64p);
    if (Input() != -4)
        return;
    strcpy(m64p->controller[m_player - 1].gamepadId, ControllerID().left(63).toAscii().constData());
    m64p->controller[m_player - 1].gamepadId[63] = '\0';
    m64p->controller[m_player - 1].button[0] = DPadR();
    m64p->controller[m_player - 1].button[1] = DPadL();
    m64p->controller[m_player - 1].button[2] = DPadD();
    m64p->controller[m_player - 1].button[3] = DPadU();
    m64p->controller[m_player - 1].button[4] = Start();
    m64p->controller[m_player - 1].button[5] = ZTrigger();
    m64p->controller[m_player - 1].button[6] = BButton();
    m64p->controller[m_player - 1].button[7] = AButton();
    m64p->controller[m_player - 1].button[8] = CButtonR();
    m64p->controller[m_player - 1].button[9] = CButtonL();
    m64p->controller[m_player - 1].button[10] = CButtonD();
    m64p->controller[m_player - 1].button[11] = CButtonU();
    m64p->controller[m_player - 1].button[12] = RTrigger();
    m64p->controller[m_player - 1].button[13] = LTrigger();
    m64p->controller[m_player - 1].axis[0].a = XAxisLeft();
    m64p->controller[m_player - 1].axis[0].b = XAxisRight();
    m64p->controller[m_player - 1].axis[1].a = YAxisUp();
    m64p->controller[m_player - 1].axis[1].b = YAxisDown();
}



