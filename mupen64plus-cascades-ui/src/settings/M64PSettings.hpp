/*
 * M64PSettings.hpp
 *
 *  Created on: Jun 28, 2014
 *      Author: Travis
 */

#ifndef M64PSETTINGS_HPP_
#define M64PSETTINGS_HPP_

#include <QObject>
#include "SettingsInternal/AudioSettings.hpp"
#include "SettingsInternal/GamepadInputSettings.hpp"
#include "SettingsInternal/GlideVideoSettings.hpp"
#include "SettingsInternal/KeyboardInputSettings.hpp"
#include "SettingsInternal/N64VideoSettings.hpp"
#include "SettingsInternal/RiceVideoSettings.hpp"
#include "SettingsInternal/TouchKeyboardInputSettings.hpp"
#include "SettingsInternal/TouchscreenInputSettings.hpp"
#include "SettingsInternal/GeneralSettings.hpp"
#include "SettingsInternal/HardwareSettings.hpp"


class M64PSettings : public QObject
{
    Q_OBJECT

    Q_PROPERTY(GeneralSettings* Settings READ Settings CONSTANT)
    Q_PROPERTY(HardwareSettings* Hardware READ Hardware CONSTANT)
    Q_PROPERTY(AudioSettings* GameAudioSettings READ GameAudioSettings CONSTANT)
    Q_PROPERTY(int VideoPlugin READ VideoPlugin WRITE VideoPlugin NOTIFY VideoPluginChanged)
    Q_PROPERTY(N64VideoSettings* N64Settings READ N64Settings CONSTANT)
    Q_PROPERTY(RiceVideoSettings* RiceSettings READ RiceSettings CONSTANT)
    Q_PROPERTY(GlideVideoSettings* GlideSettings READ GlideSettings CONSTANT)
    Q_PROPERTY(int Player1Input READ Player1Input WRITE Player1Input NOTIFY Player1InputChanged)
    Q_PROPERTY(KeyboardInputSettings* Player1KeyboardSettings READ Player1KeyboardSettings CONSTANT)
    Q_PROPERTY(TouchscreenInputSettings* Player1TouchscreenSettings READ Player1TouchscreenSettings CONSTANT)
    Q_PROPERTY(TouchKeyboardInputSettings* Player1TouchKeyboardSettings READ Player1TouchKeyboardSettings CONSTANT)
    Q_PROPERTY(GamepadInputSettings* Player1GamepadSettings READ Player1GamepadSettings CONSTANT)
    Q_PROPERTY(int Player2Input READ Player2Input WRITE Player2Input NOTIFY Player2InputChanged)
    Q_PROPERTY(KeyboardInputSettings* Player2KeyboardSettings READ Player2KeyboardSettings CONSTANT)
    Q_PROPERTY(TouchscreenInputSettings* Player2TouchscreenSettings READ Player2TouchscreenSettings CONSTANT)
    Q_PROPERTY(TouchKeyboardInputSettings* Player2TouchKeyboardSettings READ Player2TouchKeyboardSettings CONSTANT)
    Q_PROPERTY(GamepadInputSettings* Player2GamepadSettings READ Player2GamepadSettings CONSTANT)
    Q_PROPERTY(int Player3Input READ Player3Input WRITE Player3Input NOTIFY Player3InputChanged)
    Q_PROPERTY(KeyboardInputSettings* Player3KeyboardSettings READ Player3KeyboardSettings CONSTANT)
    Q_PROPERTY(TouchscreenInputSettings* Player3TouchscreenSettings READ Player3TouchscreenSettings CONSTANT)
    Q_PROPERTY(TouchKeyboardInputSettings* Player3TouchKeyboardSettings READ Player3TouchKeyboardSettings CONSTANT)
    Q_PROPERTY(GamepadInputSettings* Player3GamepadSettings READ Player3GamepadSettings CONSTANT)
    Q_PROPERTY(int Player4Input READ Player4Input WRITE Player4Input NOTIFY Player4InputChanged)
    Q_PROPERTY(KeyboardInputSettings* Player4KeyboardSettings READ Player4KeyboardSettings CONSTANT)
    Q_PROPERTY(TouchscreenInputSettings* Player4TouchscreenSettings READ Player4TouchscreenSettings CONSTANT)
    Q_PROPERTY(TouchKeyboardInputSettings* Player4TouchKeyboardSettings READ Player4TouchKeyboardSettings CONSTANT)
    Q_PROPERTY(GamepadInputSettings* Player4GamepadSettings READ Player4GamepadSettings CONSTANT)

public:
    Q_INVOKABLE int PlayerInput(int player);
    Q_INVOKABLE void PlayerInput(int player, int input);
    Q_INVOKABLE KeyboardInputSettings* PlayerKeyboardSettings(int player);
    Q_INVOKABLE TouchscreenInputSettings* PlayerTouchscreenSettings(int player);
    Q_INVOKABLE TouchKeyboardInputSettings* PlayerTouchKeyboardSettings(int player);
    Q_INVOKABLE GamepadInputSettings* PlayerGamepadSettings(int player);

    enum VideoPlugin
    {
        RICE = 0,
        N64 = 1,
        GLIDE = 2
    };

public:
    GeneralSettings* Settings() { return m_general; }
    HardwareSettings* Hardware() { return m_hardware; }
    AudioSettings* GameAudioSettings() { return m_audio; }
    int VideoPlugin() { return m_n64Video->VideoPlugin(); }
    void VideoPlugin(int val) { m_n64Video->VideoPlugin(val); }
    N64VideoSettings* N64Settings() { return m_n64Video; }
    RiceVideoSettings* RiceSettings() { return m_riceVideo; }
    GlideVideoSettings* GlideSettings() { return m_glideVideo; }
    int Player1Input() { return m_player1Keyboard->Input(); }
    void Player1Input(int val) { m_player1Keyboard->Input(val); }
    KeyboardInputSettings* Player1KeyboardSettings() { return m_player1Keyboard; }
    TouchscreenInputSettings* Player1TouchscreenSettings() { return m_player1Touchscreen; }
    TouchKeyboardInputSettings* Player1TouchKeyboardSettings() { return m_player1TouchKeyboard; }
    GamepadInputSettings* Player1GamepadSettings() { return m_player1Gamepad; }
    int Player2Input() { return m_player2Keyboard->Input(); }
    void Player2Input(int val) { m_player2Keyboard->Input(val); }
    KeyboardInputSettings* Player2KeyboardSettings() { return m_player2Keyboard; }
    TouchscreenInputSettings* Player2TouchscreenSettings() { return m_player2Touchscreen; }
    TouchKeyboardInputSettings* Player2TouchKeyboardSettings() { return m_player2TouchKeyboard; }
    GamepadInputSettings* Player2GamepadSettings() { return m_player2Gamepad; }
    int Player3Input() { return m_player3Keyboard->Input(); }
    void Player3Input(int val) { m_player3Keyboard->Input(val); }
    KeyboardInputSettings* Player3KeyboardSettings() { return m_player3Keyboard; }
    TouchscreenInputSettings* Player3TouchscreenSettings() { return m_player3Touchscreen; }
    TouchKeyboardInputSettings* Player3TouchKeyboardSettings() { return m_player3TouchKeyboard; }
    GamepadInputSettings* Player3GamepadSettings() { return m_player3Gamepad; }
    int Player4Input() { return m_player4Keyboard->Input(); }
    void Player4Input(int val) { m_player4Keyboard->Input(val); }
    KeyboardInputSettings* Player4KeyboardSettings() { return m_player4Keyboard; }
    TouchscreenInputSettings* Player4TouchscreenSettings() { return m_player4Touchscreen; }
    TouchKeyboardInputSettings* Player4TouchKeyboardSettings() { return m_player4TouchKeyboard; }
    GamepadInputSettings* Player4GamepadSettings() { return m_player4Gamepad; }

signals:
    void VideoPluginChanged();
    void Player1InputChanged();
    void Player2InputChanged();
    void Player3InputChanged();
    void Player4InputChanged();

protected slots:
    void onPerROMSettingsChanged();

public:
    M64PSettings();
    ~M64PSettings();

    void setGameName(const QString& name);

    inline void sync() { SettingsBase::sync(); }

private:
    static bool m_initialized;

    GeneralSettings *m_general;
    HardwareSettings* m_hardware;
    AudioSettings *m_audio;

    //video settings
    N64VideoSettings *m_n64Video;
    RiceVideoSettings *m_riceVideo;
    GlideVideoSettings *m_glideVideo;

    //player 1
    KeyboardInputSettings *m_player1Keyboard;
    TouchscreenInputSettings *m_player1Touchscreen;
    TouchKeyboardInputSettings *m_player1TouchKeyboard;
    GamepadInputSettings *m_player1Gamepad;

    //player 2
    KeyboardInputSettings *m_player2Keyboard;
    TouchscreenInputSettings *m_player2Touchscreen;
    TouchKeyboardInputSettings *m_player2TouchKeyboard;
    GamepadInputSettings *m_player2Gamepad;

    //player 3
    KeyboardInputSettings *m_player3Keyboard;
    TouchscreenInputSettings *m_player3Touchscreen;
    TouchKeyboardInputSettings *m_player3TouchKeyboard;
    GamepadInputSettings *m_player3Gamepad;

    //player 4
    KeyboardInputSettings *m_player4Keyboard;
    TouchscreenInputSettings *m_player4Touchscreen;
    TouchKeyboardInputSettings *m_player4TouchKeyboard;
    GamepadInputSettings *m_player4Gamepad;
};

#endif /* M64PSETTINGS_HPP_ */
