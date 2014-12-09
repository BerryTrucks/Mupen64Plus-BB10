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
    
    Container {
        leftPadding: 15
        rightPadding: 15
        DropDown {
            id: _skin
            title: qsTr("Controller Layout")
            
            Option {
                text: qsTr("None")
            }
            
            Option {
                text: qsTr("Default")
            }
            
            Option {
                text: qsTr("Alternate")
            }
            
            Option {
                text: qsTr("Fullscreen")
            }
            
            Option {
                id: customOption
                text: qsTr("Custom")
            }

            Option {
                id: dualAnalogOption
                text: qsTr("Dual Analog")
            }

            function onControllerLayoutChanged() {
                selectedIndex = _settings.PlayerTouchscreenSettings(player).ControllerLayout
            }
            
            function onChanged(selectedIndex) {
                _settings.PlayerTouchscreenSettings(player).ControllerLayout = selectedIndex
                layoutPreview.imageSource = layoutImage()
            }
            
            function layoutImage() {
                if (selectedIndex < 4) {
                    return "asset:///images/" + _skin.selectedOption.text + ".png"
                }
                else if (selectedIndex == 5) {
                    return "asset:///images/Dual.png"
                }
                else {
                    return _frontend.ImageName
                }
            }
            
            onCreationCompleted: {
                onControllerLayoutChanged()
                _settings.PlayerTouchscreenSettings(player).ControllerLayoutChanged.connect(onControllerLayoutChanged)
                selectedIndexChanged.connect(onChanged)
                layoutPreview.imageSource = layoutImage()
            }
        }
        
        Button {
            text: qsTr("Create Layout")
            horizontalAlignment: HorizontalAlignment.Fill
            visible: _skin.selectedIndex == 4
            
            function onClosed() {
                layoutPreview.imageSource = _skin.layoutImage()
            }
            
            onClicked: {
                OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayLandscape
                var sheet = overlayCreator.createObject(_frontend)
                sheet.closed.connect(onClosed)
                sheet.open()
            }
        }
    }
    
    Divider {
        visible: dualAnalogOption.selected
    }
    
    Container {
        visible: dualAnalogOption.selected
        leftPadding: 15
        rightPadding: 15
        
        DropDown {
            title: qsTr("Dual Analog Game")
            
            Option {
                text: qsTr("Star Wars Episode I: Racer")
            }
            
            Option {
                text: qsTr("Goldeneye 007")
            }
            
            function onDualAnalogGameIndexChanged() {
                selectedIndex = _settings.PlayerTouchscreenSettings(player).DualAnalogGameIndex
            }
            
            function onChanged(selectedIndex) {
                _settings.PlayerTouchscreenSettings(player).DualAnalogGameIndex = selectedIndex
            }
            
            onCreationCompleted: {
                onDualAnalogGameIndexChanged()
                _settings.PlayerTouchscreenSettings(player).DualAnalogGameIndexChanged.connect(onDualAnalogGameIndexChanged)
                selectedIndexChanged.connect(onChanged)
            }
        }
    }
    
    Container {
        leftPadding: 15
        rightPadding: 15
        topPadding: 15

        //pick starting layout
        ImageView {
            id: layoutPreview
            scalingMethod: ScalingMethod.AspectFit
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: overlayCreator
            source: "../overlayCreator.qml"
        }
    ]
}