import bb.cascades 1.0
import "../helpers"

Container {
    property int player: 1
    
    Container {
        leftPadding: 15
        rightPadding: 15
        DropDown {
            id: deviceList
            title: qsTr("Controller")
            selectedIndex: 0
            objectName: "deviceList" + player
            
            Option {
                text: qsTr("None")
                selected: true
                value: ""
            }
            
            onOptionAdded: {
                selectedIndex = _frontend.getControllerIndex(_settings.PlayerGamepadSettings(player).ControllerID)
            }
            
            function onControllerChanged() {
                selectedIndex = _frontend.getControllerIndex(_settings.PlayerGamepadSettings(player).ControllerID)
            }
            
            function setSelectedIndex(selectedIndex) {
                _settings.PlayerGamepadSettings(player).ControllerID = selectedOption.value
            }
            
            function controllersDetected() {
                selectedIndexChanged.connect(setSelectedIndex)
            }
            
            onCreationCompleted: {
                _frontend.controllersDetected.connect(controllersDetected)
                _settings.PlayerGamepadSettings(player).ControllerIDChanged.connect(onControllerChanged)
            }
        }
    }
    
    Divider {
    }

    Container {
        leftPadding: 15
        rightPadding: 15
        layout: DockLayout {

        }
        horizontalAlignment: HorizontalAlignment.Fill
        Label {
            text: qsTr("Use Controller Pak")
            verticalAlignment: VerticalAlignment.Center
        }
        ToggleButton {
            id: conPak
            horizontalAlignment: HorizontalAlignment.Right
        }
        
        function onControllerPakChanged() {
            conPak.checked = _settings.PlayerGamepadSettings(player).ControllerPak
        }
        
        function onChanged() {
            _settings.PlayerGamepadSettings(player).ControllerPak = conPak.checked
        }
        
        onCreationCompleted: {
            onControllerPakChanged()
            _settings.PlayerGamepadSettings(player).ControllerPakChanged.connect(onControllerPakChanged)
            conPak.checkedChanged.connect(onChanged)
        }
    }
    
    Divider {
    }
    
    ControllerKeyBinding {
        id: dr2
        button: qsTr("D-Pad Right")
        
        function onDPadRChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).DPadR
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).DPadR = boundKey
        }
        
        onCreationCompleted: {
            onDPadRChanged()
            _settings.PlayerGamepadSettings(player).DPadRChanged.connect(onDPadRChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: dl2
        button: qsTr("D-Pad Left")
        
        function onDPadLChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).DPadL
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).DPadL = boundKey
        }
        
        onCreationCompleted: {
            onDPadLChanged()
            _settings.PlayerGamepadSettings(player).DPadLChanged.connect(onDPadLChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: dd2
        button: qsTr("D-Pad Down")
        
        function onDPadDChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).DPadD
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).DPadD = boundKey
        }
        
        onCreationCompleted: {
            onDPadDChanged()
            _settings.PlayerGamepadSettings(player).DPadDChanged.connect(onDPadDChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: du2
        button: qsTr("D-Pad Up")
        
        function onDPadUChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).DPadU
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).DPadU = boundKey
        }
        
        onCreationCompleted: {
            onDPadUChanged()
            _settings.PlayerGamepadSettings(player).DPadUChanged.connect(onDPadUChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: start2 
        button: qsTr("Start")
        
        function onStartChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).Start
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).Start = boundKey
        }
        
        onCreationCompleted: {
            onStartChanged()
            _settings.PlayerGamepadSettings(player).StartChanged.connect(onStartChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: z2
        button: qsTr("Z Trigger")
        
        function onZTriggerChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).ZTrigger
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).ZTrigger = boundKey
        }
        
        onCreationCompleted: {
            onZTriggerChanged()
            _settings.PlayerGamepadSettings(player).ZTriggerChanged.connect(onZTriggerChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: b2 
        button: qsTr("B Button")
        
        function onBButtonChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).BButton
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).BButton = boundKey
        }
        
        onCreationCompleted: {
            onBButtonChanged()
            _settings.PlayerGamepadSettings(player).BButtonChanged.connect(onBButtonChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: a2 
        button: qsTr("A Button")
        
        function onAButtonChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).AButton
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).AButton = boundKey
        }
        
        onCreationCompleted: {
            onAButtonChanged()
            _settings.PlayerGamepadSettings(player).AButtonChanged.connect(onAButtonChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: cr2
        button: qsTr("C Button Right")
        
        function onCButtonRChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).CButtonR
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).CButtonR = boundKey
        }
        
        onCreationCompleted: {
            onCButtonRChanged()
            _settings.PlayerGamepadSettings(player).CButtonRChanged.connect(onCButtonRChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: cl2
        button: qsTr("C Button Left")
        
        function onCButtonLChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).CButtonL
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).CButtonL = boundKey
        }
        
        onCreationCompleted: {
            onCButtonLChanged()
            _settings.PlayerGamepadSettings(player).CButtonLChanged.connect(onCButtonLChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: cd2
        button: qsTr("C Button Down")
        
        function onCButtonDChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).CButtonD
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).CButtonD = boundKey
        }
        
        onCreationCompleted: {
            onCButtonDChanged()
            _settings.PlayerGamepadSettings(player).CButtonDChanged.connect(onCButtonDChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: cu2
        button: qsTr("C Button Up")
        
        function onCButtonUChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).CButtonU
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).CButtonU = boundKey
        }
        
        onCreationCompleted: {
            onCButtonUChanged()
            _settings.PlayerGamepadSettings(player).CButtonUChanged.connect(onCButtonUChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: rt2
        button: qsTr("Right Trigger")
        
        function onRTriggerChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).RTrigger
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).RTrigger = boundKey
        }
        
        onCreationCompleted: {
            onRTriggerChanged()
            _settings.PlayerGamepadSettings(player).RTriggerChanged.connect(onRTriggerChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }

    ControllerKeyBinding {
        id: lt2
        button: qsTr("Left Trigger")
        
        function onLTriggerChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).LTrigger
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).LTrigger = boundKey
        }
        
        onCreationCompleted: {
            onLTriggerChanged()
            _settings.PlayerGamepadSettings(player).LTriggerChanged.connect(onLTriggerChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }
    
    DropDown {
        id: _analogType
        title: qsTr("Analog Stick Type")
        
        options: [
            Option {
                text: qsTr("Keys")
            },
            Option {
                text: qsTr("Analog0")
            },
            Option {
                text: qsTr("Analog1")
            }
        ]
        
        function onAnalogStickTypeChanged() {
            selectedIndex = _settings.PlayerGamepadSettings(player).AnalogStickType
        }
        
        function onChange(selectedIndex) {
            _settings.PlayerGamepadSettings(player).AnalogStickType = selectedIndex
        }
        
        onCreationCompleted: {
            onAnalogStickTypeChanged()
            _settings.PlayerGamepadSettings(player).AnalogStickTypeChanged.connect(onAnalogStickTypeChanged)
            selectedIndexChanged.connect(onChange)
        }
    }
    
    Divider {
    }
    
    ControllerKeyBinding {
        id: xl2
        visible: _analogType.selectedIndex == 0
        button: qsTr("X Axis Left")
        
        function onXAxisLeftChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).XAxisLeft
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).XAxisLeft = boundKey
        }
        
        onCreationCompleted: {
            onXAxisLeftChanged()
            _settings.PlayerGamepadSettings(player).XAxisLeftChanged.connect(onXAxisLeftChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }
    
    ControllerKeyBinding {
        id: xr2
        visible: _analogType.selectedIndex == 0
        button: qsTr("X Axis Right")
        
        function onXAxisRightChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).XAxisRight
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).XAxisRight = boundKey
        }
        
        onCreationCompleted: {
            onXAxisRightChanged()
            _settings.PlayerGamepadSettings(player).XAxisRightChanged.connect(onXAxisRightChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }
    
    ControllerKeyBinding {
        id: xu2
        visible: _analogType.selectedIndex == 0
        button: qsTr("Y Axis Up")
        
        function onYAxisUpChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).YAxisUp
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).YAxisUp = boundKey
        }
        
        onCreationCompleted: {
            onYAxisUpChanged()
            _settings.PlayerGamepadSettings(player).YAxisUpChanged.connect(onYAxisUpChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }
    
    ControllerKeyBinding {
        id: xd2
        visible: _analogType.selectedIndex == 0
        button: qsTr("Y Axis Down")
        
        function onYAxisDownChanged() {
            boundKey = _settings.PlayerGamepadSettings(player).YAxisDown
        }
        
        function onKeyChanged() {
            _settings.PlayerGamepadSettings(player).YAxisDown = boundKey
        }
        
        onCreationCompleted: {
            onYAxisDownChanged()
            _settings.PlayerGamepadSettings(player).YAxisDownChanged.connect(onYAxisDownChanged)
            boundKeyChanged.connect(onKeyChanged)
        }
    }
}
