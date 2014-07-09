/*
 * M64PSettings.cpp
 *
 *  Created on: Jun 28, 2014
 *      Author: Travis
 */

#include "M64PSettings.hpp"

#include <QtDeclarative>

#include <bb/Application>


bool M64PSettings::m_initialized = false;

M64PSettings::M64PSettings()
{
    if (!m_initialized && bb::Application::instance())
    {
        m_initialized = true;
        qmlRegisterType<GeneralSettings>("mupen.settings", 1, 0, "GeneralSettings");
        qmlRegisterType<AudioSettings>("mupen.settings", 1, 0, "AudioSettings");
        qmlRegisterType<N64VideoSettings>("mupen.settings", 1, 0, "N64VideoSettings");
        qmlRegisterType<RiceVideoSettings>("mupen.settings", 1, 0, "RiceVideoSettings");
        qmlRegisterType<GlideVideoSettings>("mupen.settings", 1, 0, "GlideVideoSettings");
        qmlRegisterType<KeyboardInputSettings>("mupen.settings", 1, 0, "KeyboardInputSettings");
        qmlRegisterType<TouchscreenInputSettings>("mupen.settings", 1, 0, "TouchscreenInputSettings");
        qmlRegisterType<TouchKeyboardInputSettings>("mupen.settings", 1, 0, "TouchKeyboardInputSettings");
        qmlRegisterType<GamepadInputSettings>("mupen.settings", 1, 0, "GamepadInputSettings");
    }

    m_general = new GeneralSettings();
    m_audio = new AudioSettings();

    m_n64Video = new N64VideoSettings();
    m_riceVideo = new RiceVideoSettings();
    m_glideVideo = new GlideVideoSettings();

    connect(m_n64Video, SIGNAL(VideoPluginChanged()), SIGNAL(VideoPluginChanged()));

    m_player1Keyboard = new KeyboardInputSettings(1);
    m_player1Touchscreen = new TouchscreenInputSettings(1);
    m_player1TouchKeyboard = new TouchKeyboardInputSettings(1);
    m_player1Gamepad = new GamepadInputSettings(1);

    connect(m_player1Keyboard, SIGNAL(InputChanged()), SIGNAL(Player1InputChanged()));

    m_player2Keyboard = new KeyboardInputSettings(2);
    m_player2Touchscreen = new TouchscreenInputSettings(2);
    m_player2TouchKeyboard = new TouchKeyboardInputSettings(2);
    m_player2Gamepad = new GamepadInputSettings(2);

    connect(m_player2Keyboard, SIGNAL(InputChanged()), SIGNAL(Player2InputChanged()));

    m_player3Keyboard = new KeyboardInputSettings(3);
    m_player3Touchscreen = new TouchscreenInputSettings(3);
    m_player3TouchKeyboard = new TouchKeyboardInputSettings(3);
    m_player3Gamepad = new GamepadInputSettings(3);

    connect(m_player3Keyboard, SIGNAL(InputChanged()), SIGNAL(Player3InputChanged()));

    m_player4Keyboard = new KeyboardInputSettings(4);
    m_player4Touchscreen = new TouchscreenInputSettings(4);
    m_player4TouchKeyboard = new TouchKeyboardInputSettings(4);
    m_player4Gamepad = new GamepadInputSettings(4);

    connect(m_player4Keyboard, SIGNAL(InputChanged()), SIGNAL(Player4InputChanged()));

    connect(m_general, SIGNAL(PerROMSettingsChanged()), SLOT(onPerROMSettingsChanged()));
}

M64PSettings::~M64PSettings()
{
    delete m_audio;

    delete m_n64Video;
    delete m_riceVideo;
    delete m_glideVideo;

    delete m_player1Keyboard;
    delete m_player1Touchscreen;
    delete m_player1TouchKeyboard;
    delete m_player1Gamepad;

    delete m_player2Keyboard;
    delete m_player2Touchscreen;
    delete m_player2TouchKeyboard;
    delete m_player2Gamepad;

    delete m_player3Keyboard;
    delete m_player3Touchscreen;
    delete m_player3TouchKeyboard;
    delete m_player3Gamepad;

    delete m_player4Keyboard;
    delete m_player4Touchscreen;
    delete m_player4TouchKeyboard;
    delete m_player4Gamepad;
}

void M64PSettings::setGameName(const QString &filename)
{
    if (Settings()->PerROMSettings())
    {
        QFileInfo fl(filename);
        QString name = fl.baseName();
        name.replace(' ', '_');
        m_audio->GameName(name);
        m_n64Video->GameName(name);
        m_riceVideo->GameName(name);
        m_glideVideo->GameName(name);
        m_player1Keyboard->GameName(name);
        m_player1Touchscreen->GameName(name);
        m_player1TouchKeyboard->GameName(name);
        m_player1Gamepad->GameName(name);
        m_player2Keyboard->GameName(name);
        m_player2Touchscreen->GameName(name);
        m_player2TouchKeyboard->GameName(name);
        m_player2Gamepad->GameName(name);
        m_player3Keyboard->GameName(name);
        m_player3Touchscreen->GameName(name);
        m_player3TouchKeyboard->GameName(name);
        m_player3Gamepad->GameName(name);
        m_player4Keyboard->GameName(name);
        m_player4Touchscreen->GameName(name);
        m_player4TouchKeyboard->GameName(name);
        m_player4Gamepad->GameName(name);
    }
}

void M64PSettings::onPerROMSettingsChanged()
{
    if (!Settings()->PerROMSettings())
    {
        m_audio->GameName("");
        m_n64Video->GameName("");
        m_riceVideo->GameName("");
        m_glideVideo->GameName("");
        m_player1Keyboard->GameName("");
        m_player1Touchscreen->GameName("");
        m_player1TouchKeyboard->GameName("");
        m_player1Gamepad->GameName("");
        m_player2Keyboard->GameName("");
        m_player2Touchscreen->GameName("");
        m_player2TouchKeyboard->GameName("");
        m_player2Gamepad->GameName("");
        m_player3Keyboard->GameName("");
        m_player3Touchscreen->GameName("");
        m_player3TouchKeyboard->GameName("");
        m_player3Gamepad->GameName("");
        m_player4Keyboard->GameName("");
        m_player4Touchscreen->GameName("");
        m_player4TouchKeyboard->GameName("");
        m_player4Gamepad->GameName("");
    }
}

KeyboardInputSettings* M64PSettings::PlayerKeyboardSettings(int player)
{
    switch (player) {
        case 1:
            return Player1KeyboardSettings();
        case 2:
            return Player2KeyboardSettings();
        case 3:
            return Player3KeyboardSettings();
        case 4:
            return Player4KeyboardSettings();
        default:
            __builtin_unreachable();
    }
}

TouchscreenInputSettings* M64PSettings::PlayerTouchscreenSettings(int player)
{
    switch (player) {
        case 1:
            return Player1TouchscreenSettings();
        case 2:
            return Player2TouchscreenSettings();
        case 3:
            return Player3TouchscreenSettings();
        case 4:
            return Player4TouchscreenSettings();
        default:
            __builtin_unreachable();
    }
}

TouchKeyboardInputSettings* M64PSettings::PlayerTouchKeyboardSettings(int player)
{
    switch (player) {
        case 1:
            return Player1TouchKeyboardSettings();
        case 2:
            return Player2TouchKeyboardSettings();
        case 3:
            return Player3TouchKeyboardSettings();
        case 4:
            return Player4TouchKeyboardSettings();
        default:
            __builtin_unreachable();
    }
}

GamepadInputSettings* M64PSettings::PlayerGamepadSettings(int player)
{
    switch (player) {
        case 1:
            return Player1GamepadSettings();
        case 2:
            return Player2GamepadSettings();
        case 3:
            return Player3GamepadSettings();
        case 4:
            return Player4GamepadSettings();
        default:
            __builtin_unreachable();
    }
}

int M64PSettings::PlayerInput(int player)
{
    switch (player) {
        case 1:
            return Player1Input();
        case 2:
            return Player2Input();
        case 3:
            return Player3Input();
        case 4:
            return Player4Input();
        default:
            __builtin_unreachable();
    }
}

void M64PSettings::PlayerInput(int player, int input)
{
    switch (player) {
        case 1:
            Player1Input(input);
            break;
        case 2:
            Player2Input(input);
            break;
        case 3:
            Player3Input(input);
            break;
        case 4:
            Player4Input(input);
            break;
        default:
            __builtin_unreachable();
    }
}

