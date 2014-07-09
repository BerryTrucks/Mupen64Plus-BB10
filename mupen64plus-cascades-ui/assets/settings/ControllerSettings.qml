import bb.cascades 1.0

Container {
    property int player: 1
    topPadding: 15
    property int controllerType: 0
    
    function clear() {
        if (_keyboard.selected) {
            _settings.PlayerKeyboardSettings(player).clear()
        }
        else if (_gamepad.selected) {
            _settings.PlayerGamepadSettings(player).clear()
        }
        else if (_touchscreen.selected) {
            _settings.PlayerTouchscreenSettings(player).clear()
        }
        else if (_touchkeyboard.selected) {
            _settings.PlayerTouchKeyboardSettings(player).clear()
        }
    }

    function reset() {
        if (_keyboard.selected) {
            _settings.PlayerKeyboardSettings(player).reset()
        }
        else if (_gamepad.selected) {
            _settings.PlayerGamepadSettings(player).reset()
        }
        else if (_touchscreen.selected) {
            _settings.PlayerTouchscreenSettings(player).reset()
        }
        else if (_touchkeyboard.selected) {
            _settings.PlayerTouchKeyboardSettings(player).reset()
        }
    }
    
    ScrollView {
        Container {
            Container {
                leftPadding: 15
                rightPadding: 15
                DropDown {
                    id: _device
                    title: "Input Device"
                    
                    Option {
                        id: _disabled
                        text: "Disabled"
                        value: 0
                    }
                    Option {
                        id: _keyboard
                        text: "Keyboard"
                        value: -2
                    }
                    Option {
                        id: _gamepad
                        text: "GamePad"
                        value: -4
                    }
                    
                    attachedObjects: [
                        Option {
                            id: _touchkeyboard
                            text: "Touchscreen/Keyboard"
                            value: -5
                        },
                        Option {
                            id: _touchscreen
                            text: "Touchscreen"
                            value: -3
                        }
                    ]
                    
                    function onInputChanged() {
                        controllerType = _settings.PlayerInput(player)
                        if (controllerType == 0) {
                            _disabled.selected = true
                        }
                        else if (controllerType == -2) {
                            _keyboard.selected = true
                        }
                        else if (controllerType == -3) {
                            _touchscreen.selected = true
                        }
                        else if (controllerType == -4) {
                            _gamepad.selected = true
                        }
                        else if (controllerType == -5) {
                            _touchkeyboard.selected = true
                        }
                    }
                    
                    function onIndexChanged(selectedIndex) {
                        _settings.PlayerInput(player, selectedOption.value)
                        controllerType = selectedOption.value
                    }
                    
                    onCreationCompleted: {
                        if (_frontend.Keyboard) {
                            add(_touchkeyboard)
                        }
                        else {
                            insert(1, _touchscreen)
                        }
                        onInputChanged()
                        _settings.PlayerKeyboardSettings(player).InputChanged.connect(onInputChanged)
                        selectedIndexChanged.connect(onIndexChanged)
                    }
                }
            }
            Divider {
            }
            
            KeyboardSettings {
                player: player
                visible: _keyboard.selected
            }
            
            TouchscreenSettings {
                player: player
                visible: _touchscreen.selected
            }
            
            TouchKeyboardSettings {
                player: player
                visible: _touchkeyboard.selected
            }
            
            GamepadSettings {
                player: player
                visible: _gamepad.selected
            }
            
            onCreationCompleted: {
                if (_frontend.isOSThree) {
                    bottomPadding = 55
                }
            }
        }
    }
}
