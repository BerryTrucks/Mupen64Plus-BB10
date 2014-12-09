import bb.cascades 1.0
import mupen.settings 1.0
import "../helpers"

Sheet {
    id: settingsSheet
    peekEnabled: false

    Page {
        titleBar: TitleBar {
            id: titleBar4
            scrollBehavior: TitleBarScrollBehavior.Sticky
            title: qsTr("App Settings")
            dismissAction: ActionItem {
                title: qsTr("Close")
                
                onTriggered: {
                    settingsSheet.close()
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
                
                SettingsToggle {
                    title: qsTr("Check for Updates at Startup")
                    showHelp: help.helpEnabled
                    helpText: qsTr("Check for updates everytime the app is openned.")
                    
                    function onCheckVersionChanged() {
                        checked = _settings.Settings.CheckVersion
                    }
                    
                    function onChanged() {
                        _settings.Settings.CheckVersion = checked
                    }
                    
                    onCreationCompleted: {
                        onCheckVersionChanged()
                        _settings.Settings.CheckVersionChanged.connect(onCheckVersionChanged)
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
                            var sheet = themeSheet.createObject(_frontend)
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
                
                Divider {
                }
                
                onCreationCompleted: {
                    if (_frontend.isOSThree) {
                        bottomPadding = 55
                    }
                }
            }
        }
    }
}
