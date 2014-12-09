import bb.cascades 1.0
import "../helpers"

Container {
    property int player: 1
    
    Container {
        leftPadding: 15
        rightPadding: 15
        
        DropDown {
            title: qsTr("Rotation Direction")
            
            Option {
                text: qsTr("Clockwise")
            }
            Option {
                text: qsTr("Counter-Clockwise")
            }
            
            function onRotationDirectionChanged() {
                selectedIndex = _settings.PlayerTouchKeyboardSettings(player).RotationDirection
            }
            
            function onChanged(selectedIndex) {
                _settings.PlayerTouchKeyboardSettings(player).RotationDirection = selectedIndex
            }
            
            onCreationCompleted: {
                onRotationDirectionChanged()
                _settings.PlayerTouchKeyboardSettings(player).RotationDirectionChanged.connect(onRotationDirectionChanged)
                selectedIndexChanged.connect(onChanged)
            }
        }
    }
    
    Divider {
    }
    
    Container {
        leftPadding: 15.0
        rightPadding: 15.0
        horizontalAlignment: HorizontalAlignment.Fill
        DropDown {
            title: qsTr("Controller Plugin")
            Option {
                text: qsTr("None")
            }
            Option {
                text: qsTr("Controller Pak")
            }
            Option {
                text: qsTr("Rumble Pak")
            }
            
            function onRumblePakChanged() {
                if (!_settings.PlayerTouchscreenSettings(player).ControllerPak) {
                    if (_settings.PlayerTouchscreenSettings(player).RumblePak) {
                        selectedIndex = 2
                    }
                    else {
                        selectedIndex = 0
                    }
                }
            }
            
            function onControllerPakChanged() {
                if (_settings.PlayerTouchscreenSettings(player).ControllerPak) {
                    selectedIndex = 1
                }
                else {
                    selectedIndex = 0
                }
            }
            
            function onChanged() {
                if (selectedIndex == 0) {
                    _settings.PlayerTouchscreenSettings(player).ControllerPak = false
                    _settings.PlayerTouchscreenSettings(player).RumblePak = false
                }
                else if (selectedIndex == 1) {
                    _settings.PlayerTouchscreenSettings(player).ControllerPak = true
                    _settings.PlayerTouchscreenSettings(player).RumblePak = false
                }
                else {
                    _settings.PlayerTouchscreenSettings(player).ControllerPak = false
                    _settings.PlayerTouchscreenSettings(player).RumblePak = true
                }
            }
            
            onCreationCompleted: {
                onControllerPakChanged()
                onRumblePakChanged()
                _settings.PlayerTouchscreenSettings(player).RumblePakChanged.connect(onRumblePakChanged)
                _settings.PlayerTouchscreenSettings(player).ControllerPakChanged.connect(onControllerPakChanged)
                selectedIndexChanged.connect(onChanged)
            }
        }
    }
    
    Divider {
    }
    
    KeyBinding {
        button: qsTr("D-Pad Right")
        
        function onDPadRChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).DPadR
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).DPadR = boundKey
        }
        
        onCreationCompleted: {
            onDPadRChanged()
            _settings.PlayerTouchKeyboardSettings(player).DPadRChanged.connect(onDPadRChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("D-Pad Left")
        
        function onDPadLChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).DPadL
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).DPadL = boundKey
        }
        
        onCreationCompleted: {
            onDPadLChanged()
            _settings.PlayerTouchKeyboardSettings(player).DPadLChanged.connect(onDPadLChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("D-Pad Up")
        
        function onDPadUChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).DPadU
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).DPadU = boundKey
        }
        
        onCreationCompleted: {
            onDPadUChanged()
            _settings.PlayerTouchKeyboardSettings(player).DPadUChanged.connect(onDPadUChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("D-Pad Down")
        
        function onDPadDChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).DPadD
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).DPadD = boundKey
        }
        
        onCreationCompleted: {
            onDPadDChanged()
            _settings.PlayerTouchKeyboardSettings(player).DPadDChanged.connect(onDPadDChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Z Trigger")
        
        function onZTriggerChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).ZTrigger
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).ZTrigger = boundKey
        }
        
        onCreationCompleted: {
            onZTriggerChanged()
            _settings.PlayerTouchKeyboardSettings(player).ZTriggerChanged.connect(onZTriggerChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("B Button")
        
        function onBButtonChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).BButton
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).BButton = boundKey
        }
        
        onCreationCompleted: {
            onBButtonChanged()
            _settings.PlayerTouchKeyboardSettings(player).BButtonChanged.connect(onBButtonChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("A Button")
        
        function onAButtonChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).AButton
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).AButton = boundKey
        }
        
        onCreationCompleted: {
            onAButtonChanged()
            _settings.PlayerTouchKeyboardSettings(player).AButtonChanged.connect(onAButtonChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("C Button Right")
        
        function onCButtonRChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).CButtonR
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).CButtonR = boundKey
        }
        
        onCreationCompleted: {
            onCButtonRChanged()
            _settings.PlayerTouchKeyboardSettings(player).CButtonRChanged.connect(onCButtonRChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("C Button Left")
        
        function onCButtonLChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).CButtonL
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).CButtonL = boundKey
        }
        
        onCreationCompleted: {
            onCButtonLChanged()
            _settings.PlayerTouchKeyboardSettings(player).CButtonLChanged.connect(onCButtonLChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("C Button Up")
        
        function onCButtonUChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).CButtonU
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).CButtonU = boundKey
        }
        
        onCreationCompleted: {
            onCButtonUChanged()
            _settings.PlayerTouchKeyboardSettings(player).CButtonUChanged.connect(onCButtonUChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("C Button Down")
        
        function onCButtonDChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).CButtonD
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).CButtonD = boundKey
        }
        
        onCreationCompleted: {
            onCButtonDChanged()
            _settings.PlayerTouchKeyboardSettings(player).CButtonDChanged.connect(onCButtonDChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Right Trigger")
        
        function onRTriggerChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).RTrigger
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).RTrigger = boundKey
        }
        
        onCreationCompleted: {
            onRTriggerChanged()
            _settings.PlayerTouchKeyboardSettings(player).RTriggerChanged.connect(onRTriggerChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Left Trigger")
        
        function onLTriggerChanged() {
            boundKey = _settings.PlayerTouchKeyboardSettings(player).LTrigger
        }
        
        function onChanged() {
            _settings.PlayerTouchKeyboardSettings(player).LTrigger = boundKey
        }
        
        onCreationCompleted: {
            onLTriggerChanged()
            _settings.PlayerTouchKeyboardSettings(player).LTriggerChanged.connect(onLTriggerChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
}
