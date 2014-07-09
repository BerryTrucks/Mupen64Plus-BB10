import bb.cascades 1.0
import "../helpers"

Container {
    property int player: 1
    
    Container {
        leftPadding: 15
        rightPadding: 15
        layout: DockLayout {
        }
        
        horizontalAlignment: HorizontalAlignment.Fill
        Label {
            text: qsTr("Rumble Pak")
            verticalAlignment: VerticalAlignment.Center
        }
        
        ToggleButton {
            id: toggle2
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Right

            function onRumblePakChanged() {
                checked = _settings.PlayerTouchscreenSettings(player).RumblePak
            }

            function onChanged(checked) {
                _settings.PlayerTouchscreenSettings(player).RumblePak = checked
            }

            onCreationCompleted: {
                onRumblePakChanged()
                _settings.PlayerTouchscreenSettings(player).RumblePakChanged.connect(onRumblePakChanged)
                checkedChanged.connect(onChanged)
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
            title: "Controller Layout"
            
            Option {
                text: "None"
            }
            
            Option {
                text: "Default"
            }
            
            Option {
                text: "Alternate"
            }
            
            Option {
                text: "Fullscreen"
            }
            
            function onControllerLayoutChanged() {
                selectedIndex = _settings.PlayerTouchscreenSettings(player).ControllerLayout
            }
            
            function onChanged(selectedIndex) {
                _settings.PlayerTouchscreenSettings(player).ControllerLayout = selectedIndex
            }
            
            onCreationCompleted: {
                onControllerLayoutChanged()
                _settings.PlayerTouchscreenSettings(player).ControllerLayoutChanged.connect(onControllerLayoutChanged)
                selectedIndexChanged.connect(onChanged)
            }
        }
        
        //pick starting layout
        ImageView {
            imageSource: "asset:///images/" + _skin.selectedOption.text + ".png"
            scalingMethod: ScalingMethod.AspectFit
        }
    }
}