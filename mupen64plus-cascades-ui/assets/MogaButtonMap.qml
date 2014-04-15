import bb.cascades 1.0

Container {
    property string button: "Undefined"

    horizontalAlignment: HorizontalAlignment.Fill
    Container {
        layout: DockLayout {
        }

        horizontalAlignment: HorizontalAlignment.Fill
        Label {
            verticalAlignment: VerticalAlignment.Center
            text: button
        }
        DropDown {
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            maxWidth: 400.0
            Option {
                text: "None"
                selected: true
            }
            Option {
                text: "A"
            }
            Option {
                text: "B"
            }
            /*Option {
                text: "4"
            }*/
            Option {
                text: "R3"
                description: "Right Analog Click"
            }
            Option {
                text: "X"
            }
            /*Option {
                text: "32"
            }*/
            Option {
                text: "Select"
            }
            Option {
                text: "Start"
            }
            /*Option {
                text: "256"
            }
            Option {
                text: "512"
            }*/
            Option {
                text: "L1"
                description: "Left Bumper"
            }
            Option {
                text: "L2"
                description: "Left Trigger"
            }
            Option {
                text: "L3"
                description: "Left Analog Click"
            }
            Option {
                text: "R1"
                description: "Right Bumper"
            }
            Option {
                text: "R2"
                description: "Right Trigger"
            }
            Option {
                text: "Y"
            }
            Option {
                text: "D-Pad Up"
            }
            Option {
                text: "D-Pad Down"
            }
            Option {
                text: "D-Pad Left"
            }
            Option {
                text: "D-Pad Right"
            }
            
            onCreationCompleted: {
                selectedIndex = _frontend.getMogaInputValue(player, button)
                selectedIndexChanged.connect(indexChanged)
            }
            
            function indexChanged(index) {
                _frontend.setMogaInputValue(player, button, index)
            }
        }
    }
    Divider {
    }

    function reset() {
        _frontend.setMogaInputValue(player, button, 0)
    }
}
