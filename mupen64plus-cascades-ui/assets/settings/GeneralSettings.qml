import bb.cascades 1.0
import mupen.settings 1.0
import "../helpers"

Page {
    titleBar: TitleBar {
        id: titleBar4
        scrollBehavior: TitleBarScrollBehavior.Sticky
        kind: TitleBarKind.FreeForm
        
        kindProperties: FreeFormTitleBarKindProperties {
            id: titlebarProperties
            Container {
                layout: DockLayout {
                }
                
                leftPadding: 15.0
                rightPadding: 15.0
                Label {
                    text: qsTr("General Settings")
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
                    id: titleImage
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    imageSource: "asset:///images/ic_settings.png"
                    
                    onCreationCompleted: {
                        if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                            imageSource = "asset:///images/ic_settings_dk.png"
                        }
                    }
                }
            }
        }
    }

    actions: [
        ShowHelpActionItem {
            id: help
        }
    ]
    
    ScrollView {
        Container {
            SettingsToggle {
                title: qsTr("Default to SD Card")
                showHelp: help.helpEnabled
                helpText: qsTr("When choosing a ROM, the SD Card will be the default location to search.")
                
                function onSDCardChanged() {
                    checked = _settings.Settings.DefaultToSDCard
                }
                
                function onChanged() {
                    _settings.Settings.DefaultToSDCard = checked
                }
                
                onCreationCompleted: {
                    onSDCardChanged()
                    _settings.Settings.DefaultToSDCardChanged.connect(onSDCardChanged)
                    checkedChanged.connect(onChanged)
                }
            }
            
            SettingsToggle {
                title: qsTr("Per ROM Settings")
                showHelp: help.helpEnabled
                helpText: qsTr("When a ROM is selected, remember the values previously set for it.")
                
                function onPerROMSettingsChanged() {
                    checked = _settings.Settings.PerROMSettings
                }
                
                function onChanged() {
                    _settings.Settings.PerROMSettings = checked
                }
                
                onCreationCompleted: {
                    onPerROMSettingsChanged()
                    _settings.Settings.PerROMSettingsChanged.connect(onPerROMSettingsChanged)
                    checkedChanged.connect(onChanged)
                }
            }
            
            SettingsToggle {
                title: qsTr("Box Art Scraping")
                showHelp: help.helpEnabled
                helpText: qsTr("Box art for ROMs will be automatically downloaded and displayed.")
                
                function onBoxartScrapingChanged() {
                    checked = _settings.Settings.BoxartScraping
                }
                
                function onChanged() {
                    _settings.Settings.BoxartScraping = checked
                }
                
                onCreationCompleted: {
                    onBoxartScrapingChanged()
                    _settings.Settings.BoxartScrapingChanged.connect(onBoxartScrapingChanged)
                    checkedChanged.connect(onChanged)
                }
            }
            
            SettingsToggle {
                title: qsTr("Save History")
                showHelp: help.helpEnabled
                helpText: qsTr("Played games will be remembered for easier access.")
                
                function onSaveHistoryChanged() {
                    checked = _settings.Settings.SaveHistory
                }
                
                function onChanged() {
                    _settings.Settings.SaveHistory = checked
                }
                
                onCreationCompleted: {
                    onSaveHistoryChanged()
                    _settings.Settings.SaveHistoryChanged.connect(onSaveHistoryChanged)
                    checkedChanged.connect(onChanged)
                }
            }
            
            SettingsToggle {
                title: qsTr("Show FPS")
                showHelp: help.helpEnabled
                helpText: qsTr("Show a graph of the emulators frames per second.")
                
                function onShowFPSChanged() {
                    checked = _settings.Settings.ShowFPS
                }
                
                function onChanged() {
                    _settings.Settings.ShowFPS = checked
                }
                
                onCreationCompleted: {
                    onShowFPSChanged()
                    _settings.Settings.ShowFPSChanged.connect(onShowFPSChanged)
                    checkedChanged.connect(onChanged)
                }
            }
            
            Container {
                visible: !_frontend.isOSThree
                leftPadding: 15
                rightPadding: 15
                topPadding: 20
                DropDown {
                    horizontalAlignment: HorizontalAlignment.Center
                    title: qsTr("Visual Theme")
                    Option {
                        text: qsTr("Bright")
                    }
                    Option {
                        text: qsTr("Dark")
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
                Container {
                    visible: help.helpEnabled
                    Label {
                        text: qsTr("Restart the emulator for the colour theme change to take effect.")
                        multiline: true
                    }
                }
            }
            Container {
                visible: _frontend.isOSThree
                leftPadding: 15
                rightPadding: 15
                topPadding: 20
                horizontalAlignment: HorizontalAlignment.Fill
                Button {
                    horizontalAlignment: HorizontalAlignment.Fill
                    text: qsTr("Set Visual Style")
                    
                    onClicked: {
                        var sheet = themeSheet.createObject()
                        sheet.open()
                    }
                    
                    attachedObjects: [
                        ComponentDefinition {
                            id: themeSheet
                            source: "ThemeSettings.qml"
                        }
                    ]
                }
            }
            
            onCreationCompleted: {
                if (_frontend.isOSThree) {
                    bottomPadding = 55
                }
            }
        }
    }
}
