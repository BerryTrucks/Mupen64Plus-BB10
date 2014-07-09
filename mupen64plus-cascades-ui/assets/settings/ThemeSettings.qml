import bb.cascades 1.0
import mupen.settings 1.0
import "../helpers"

Sheet {
    id: root
    peekEnabled: false

    Page {
        actions: [
            ActionItem {
                title: qsTr("Set Defaults")
                imageSource: "asset:///images/default_icon.png"
                ActionBar.placement: ActionBarPlacement.OnBar
                onTriggered: {
                    _settings.Settings.reset()
                }
            }
        ]
        
        titleBar: TitleBar {
            title: qsTr("Theme Settings")
            scrollBehavior: TitleBarScrollBehavior.Sticky
            dismissAction: ActionItem {
                title: qsTr("Close")

                onTriggered: {
                    root.close()
                }
            }
        }
        ScrollView {
            Container {
                topPadding: 15
                Container {
                    leftPadding: 15
                    rightPadding: 15
                    visible: !_frontend.isOSThreeCompiled
                    Label {
                        text: qsTr("Restart the Translator app for the colour theme change to take effect.")
                        multiline: true
                    }
                }
                Divider {
                    visible: !_frontend.isOSThreeCompiled
                }
                Container {
                    leftPadding: 15
                    rightPadding: 15
                    DropDown {
                        title: qsTr("Visual Theme")
                        horizontalAlignment: HorizontalAlignment.Center

                        Option {
                            description: qsTr("Bright")
                        }
                        Option {
                            description: qsTr("Dark")
                        }
                        
                        function onThemeChanged() {
                            selectedIndex = _settings.Settings.Theme
                        }
                        
                        function onChanged(selectedIndex) {
                            _settings.Settings.Theme = selectedIndex
                        }
                        
                        onCreationCompleted: {
                            onThemeChanged()
                            _settings.Settings.ThemeChanged.connect(onThemeChanged)
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
                        id: _primary
                        horizontalAlignment: HorizontalAlignment.Center
                        title: qsTr("Primary Colour")
                        Option {
                            text: qsTr("Default")
                        }
                        Option {
                            description: qsTr("Custom")
                        }
                        
                        function onPrimaryIndexChanged() {
                            selectedIndex = _settings.Settings.PrimaryColourIndex
                        }
                        
                        function onChanged(selectedIndex) {
                            _settings.Settings.PrimaryColourIndex = selectedIndex
                            if (selectedIndex == 0) {
                                primaryColour.red = 255
                                primaryColour.green = 0
                                primaryColour.blue = 0
                            }
                        }
                        
                        onCreationCompleted: {
                            onPrimaryIndexChanged()
                            _settings.Settings.PrimaryColourIndexChanged.connect(onPrimaryIndexChanged)
                            selectedIndexChanged.connect(onChanged)
                        }
                    }
                    
                    ColourChooser {
                        id: primaryColour
                        visible: _primary.selectedIndex == 1
                        
                        function onPrimaryRedChanged() {
                            red = _settings.Settings.PrimaryColourRed
                        }
                        
                        function onPrimaryGreenChanged() {
                            green = _settings.Settings.PrimaryColourGreen
                        }
                        
                        function onPrimaryBlueChanged() {
                            blue = _settings.Settings.PrimaryColourBlue
                        }
                        
                        function onRChanged() {
                            _settings.Settings.PrimaryColourRed = red
                        }
                        
                        function onGChanged() {
                            _settings.Settings.PrimaryColourGreen = green
                        }
                        
                        function onBChanged() {
                            _settings.Settings.PrimaryColourBlue = blue
                        }
                        
                        onCreationCompleted: {
                            onPrimaryRedChanged()
                            onPrimaryGreenChanged()
                            onPrimaryBlueChanged()
                            _settings.Settings.PrimaryColourRedChanged.connect(onPrimaryRedChanged)
                            _settings.Settings.PrimaryColourGreenChanged.connect(onPrimaryGreenChanged)
                            _settings.Settings.PrimaryColourBlueChanged.connect(onPrimaryBlueChanged)
                            redChanged.connect(onRChanged)
                            greenChanged.connect(onGChanged)
                            blueChanged.connect(onBChanged)
                        }
                    }
                }
                Divider {
                    visible: _primary.selectedIndex == 1
                }
                Container {
                    leftPadding: 15
                    rightPadding: 15
                    visible: _primary.selectedIndex == 1
                    DropDown {
                        id: _base
                        horizontalAlignment: HorizontalAlignment.Center
                        title: qsTr("Primary Base Colour")
                        Option {
                            text: qsTr("Default")
                        }
                        Option {
                            description: qsTr("Custom")
                        }
                        
                        function onBaseIndexChanged() {
                            selectedIndex = _settings.Settings.BaseColourIndex
                        }
                        
                        function onChanged(selectedIndex) {
                            _settings.Settings.BaseColourIndex = selectedIndex
                            if (selectedIndex == 0) {
                                baseColour.red = 255
                                baseColour.green = 0
                                baseColour.blue = 0
                            }
                        }
                        
                        onCreationCompleted: {
                            onBaseIndexChanged()
                            _settings.Settings.BaseColourIndexChanged.connect(onBaseIndexChanged)
                            selectedIndexChanged.connect(onChanged)
                        }
                    }
                    
                    ColourChooser {
                        id: baseColour
                        visible: _base.selectedIndex == 1
                        
                        function onBaseRedChanged() {
                            red = _settings.Settings.BaseColourRed
                        }
                        
                        function onBaseGreenChanged() {
                            green = _settings.Settings.BaseColourGreen
                        }
                        
                        function onBaseBlueChanged() {
                            blue = _settings.Settings.BaseColourBlue
                        }
                        
                        function onRChanged() {
                            _settings.Settings.BaseColourRed = red
                        }
                        
                        function onGChanged() {
                            _settings.Settings.BaseColourGreen = green
                        }
                        
                        function onBChanged() {
                            _settings.Settings.BaseColourBlue = blue
                        }
                        
                        onCreationCompleted: {
                            onBaseRedChanged()
                            onBaseGreenChanged()
                            onBaseBlueChanged()
                            _settings.Settings.BaseColourRedChanged.connect(onBaseRedChanged)
                            _settings.Settings.BaseColourGreenChanged.connect(onBaseGreenChanged)
                            _settings.Settings.BaseColourBlueChanged.connect(onBaseBlueChanged)
                            redChanged.connect(onRChanged)
                            greenChanged.connect(onGChanged)
                            blueChanged.connect(onBChanged)
                        }
                    }
                }
            }
        }
    }
}