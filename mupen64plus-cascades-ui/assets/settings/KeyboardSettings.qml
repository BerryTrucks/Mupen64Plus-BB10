import bb.cascades 1.0
import "../helpers"

Container {
    property int player: 1
    
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
            boundKey = _settings.PlayerKeyboardSettings(player).DPadR
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).DPadR = boundKey
        }
        
        onCreationCompleted: {
            onDPadRChanged()
            _settings.PlayerKeyboardSettings(player).DPadRChanged.connect(onDPadRChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("D-Pad Left")
        
        function onDPadLChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).DPadL
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).DPadL = boundKey
        }
        
        onCreationCompleted: {
            onDPadLChanged()
            _settings.PlayerKeyboardSettings(player).DPadLChanged.connect(onDPadLChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("D-Pad Up")
        
        function onDPadUChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).DPadU
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).DPadU = boundKey
        }
        
        onCreationCompleted: {
            onDPadUChanged()
            _settings.PlayerKeyboardSettings(player).DPadUChanged.connect(onDPadUChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("D-Pad Down")
        
        function onDPadDChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).DPadD
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).DPadD = boundKey
        }
        
        onCreationCompleted: {
            onDPadDChanged()
            _settings.PlayerKeyboardSettings(player).DPadDChanged.connect(onDPadDChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Start")
        
        function onStartChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).Start
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).Start = boundKey
        }
        
        onCreationCompleted: {
            onStartChanged()
            _settings.PlayerKeyboardSettings(player).StartChanged.connect(onStartChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Z Trigger")
        
        function onZTriggerChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).ZTrigger
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).ZTrigger = boundKey
        }
        
        onCreationCompleted: {
            onZTriggerChanged()
            _settings.PlayerKeyboardSettings(player).ZTriggerChanged.connect(onZTriggerChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("B Button")
        
        function onBButtonChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).BButton
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).BButton = boundKey
        }
        
        onCreationCompleted: {
            onBButtonChanged()
            _settings.PlayerKeyboardSettings(player).BButtonChanged.connect(onBButtonChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("A Button")
        
        function onAButtonChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).AButton
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).AButton = boundKey
        }
        
        onCreationCompleted: {
            onAButtonChanged()
            _settings.PlayerKeyboardSettings(player).AButtonChanged.connect(onAButtonChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("C Button Right")
        
        function onCButtonRChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).CButtonR
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).CButtonR = boundKey
        }
        
        onCreationCompleted: {
            onCButtonRChanged()
            _settings.PlayerKeyboardSettings(player).CButtonRChanged.connect(onCButtonRChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("C Button Left")
        
        function onCButtonLChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).CButtonL
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).CButtonL = boundKey
        }
        
        onCreationCompleted: {
            onCButtonLChanged()
            _settings.PlayerKeyboardSettings(player).CButtonLChanged.connect(onCButtonLChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("C Button Up")
        
        function onCButtonUChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).CButtonU
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).CButtonU = boundKey
        }
        
        onCreationCompleted: {
            onCButtonUChanged()
            _settings.PlayerKeyboardSettings(player).CButtonUChanged.connect(onCButtonUChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("C Button Down")
        
        function onCButtonDChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).CButtonD
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).CButtonD = boundKey
        }
        
        onCreationCompleted: {
            onCButtonDChanged()
            _settings.PlayerKeyboardSettings(player).CButtonDChanged.connect(onCButtonDChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Right Trigger")
        
        function onRTriggerChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).RTrigger
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).RTrigger = boundKey
        }
        
        onCreationCompleted: {
            onRTriggerChanged()
            _settings.PlayerKeyboardSettings(player).RTriggerChanged.connect(onRTriggerChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Left Trigger")
        
        function onLTriggerChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).LTrigger
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).LTrigger = boundKey
        }
        
        onCreationCompleted: {
            onLTriggerChanged()
            _settings.PlayerKeyboardSettings(player).LTriggerChanged.connect(onLTriggerChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("X Axis Right")
        
        function onXAxisRightChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).XAxisRight
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).XAxisRight = boundKey
        }
        
        onCreationCompleted: {
            onXAxisRightChanged()
            _settings.PlayerKeyboardSettings(player).XAxisRightChanged.connect(onXAxisRightChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("X Axis Left")
        
        function onXAxisLeftChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).XAxisLeft
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).XAxisLeft = boundKey
        }
        
        onCreationCompleted: {
            onXAxisLeftChanged()
            _settings.PlayerKeyboardSettings(player).XAxisLeftChanged.connect(onXAxisLeftChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Y Axis Up")
        
        function onYAxisUpChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).YAxisUp
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).YAxisUp = boundKey
        }
        
        onCreationCompleted: {
            onYAxisUpChanged()
            _settings.PlayerKeyboardSettings(player).YAxisUpChanged.connect(onYAxisUpChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Y Axis Down")
        
        function onYAxisDownChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).YAxisDown
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).YAxisDown = boundKey
        }
        
        onCreationCompleted: {
            onYAxisDownChanged()
            _settings.PlayerKeyboardSettings(player).YAxisDownChanged.connect(onYAxisDownChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Up Left")
        
        function onUpLeftChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).UpLeft
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).UpLeft = boundKey
        }
        
        onCreationCompleted: {
            onUpLeftChanged()
            _settings.PlayerKeyboardSettings(player).UpLeftChanged.connect(onUpLeftChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Up Right")
        
        function onUpRightChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).UpRight
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).UpRight = boundKey
        }
        
        onCreationCompleted: {
            onUpRightChanged()
            _settings.PlayerKeyboardSettings(player).UpRightChanged.connect(onUpRightChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Down Left")
        
        function onDownLeftChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).DownLeft
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).DownLeft = boundKey
        }
        
        onCreationCompleted: {
            onDownLeftChanged()
            _settings.PlayerKeyboardSettings(player).DownLeftChanged.connect(onDownLeftChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
    
    KeyBinding {
        button: qsTr("Down Right")
        
        function onDownRightChanged() {
            boundKey = _settings.PlayerKeyboardSettings(player).DownRight
        }
        
        function onChanged() {
            _settings.PlayerKeyboardSettings(player).DownRight = boundKey
        }
        
        onCreationCompleted: {
            onDownRightChanged()
            _settings.PlayerKeyboardSettings(player).DownRightChanged.connect(onDownRightChanged)
            boundKeyChanged.connect(onChanged)
        }
    }
}
