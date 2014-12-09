import bb.cascades 1.0
import "../helpers"

Page {
    property alias helpEnabled: help.helpEnabled
    
    actions: [
        ShowHelpActionItem {
            id: help
        }
    ]
    
    titleBar: TitleBar {
        id: titleBar4
        scrollBehavior: TitleBarScrollBehavior.Sticky
        kind: TitleBarKind.FreeForm
        
        kindProperties: FreeFormTitleBarKindProperties {
            Container {
                layout: DockLayout {
                }
                
                leftPadding: 15.0
                rightPadding: 15.0
                Label {
                    text: qsTr("Hardware Settings")
                    textStyle.fontSize: FontSize.Large
                    textStyle.fontWeight: FontWeight.W500
                    
                    onCreationCompleted: {
                        if (!_frontend.isOSThree && _settings.Settings.Theme == 0) {
                            textStyle.color = Color.White
                        }
                    }
                    verticalAlignment: VerticalAlignment.Center
                }
                
                ImageView {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    imageSource: "asset:///images/hardware.png"

                    onCreationCompleted: {
                        if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                            imageSource = "asset:///images/hardware_dk.png"
                        }
                    }
                }
            }
        }
    }
    
    Container {
        topPadding: 20
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            leftPadding: 15
            rightPadding: 15

            layout: DockLayout {
            }

            Label {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                text: qsTr("Use Expansion Pak")
            }

            ToggleButton {
                horizontalAlignment: HorizontalAlignment.Right
                function onExpansionEnabledChanged() {
                    checked = _settings.Hardware.ExpansionPak
                }

                function onChanged(checked) {
                    _settings.Hardware.ExpansionPak = checked
                }

                onCreationCompleted: {
                    onExpansionEnabledChanged()
                    _settings.Hardware.ExpansionPakChanged.connect(onExpansionEnabledChanged)
                    checkedChanged.connect(onChanged)
                }
            }
            
            Label {
                id: _help
                text: qsTr("Enable or disable the extra 4MB of RAM contained in the Expansion Pak.")
                multiline: true
                visible: helpEnabled
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    fontStyle: FontStyle.Italic
                }
            }
        }

        Divider {
        }
        
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            leftPadding: 15
            rightPadding: 15
            DropDown {
                title: qsTr("R4300 Emulation")
                Option {
                    text: qsTr("Pure Interpreter")
                }
                Option {
                    text: qsTr("Cached Interpreter")
                }
                Option {
                    text: qsTr("Dynamic Recompiler")
                }
                
                function onEmulationChanged() {
                    selectedIndex = _settings.Hardware.R4300Emulation
                }
                
                function onChanged(selectedIndex) {
                    _settings.Hardware.R4300Emulation = selectedIndex
                }
                
                onCreationCompleted: {
                    onEmulationChanged()
                    _settings.Hardware.R4300EmulationChanged.connect(onEmulationChanged)
                    selectedIndexChanged.connect(onChanged)
                }
            }
            
            Label {
                id: _help2
                text: qsTr("The method used to emulate the N64s R4300 CPU.")
                multiline: true
                visible: helpEnabled
                textStyle {
                    base: SystemDefaults.TextStyles.BodyText
                    fontStyle: FontStyle.Italic
                }
            }
        }
    }
}
