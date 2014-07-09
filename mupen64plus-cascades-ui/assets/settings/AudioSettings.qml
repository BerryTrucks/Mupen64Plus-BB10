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
                    text: qsTr("Audio Settings")
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
                    imageSource: "asset:///images/sound.png"
                    
                    onCreationCompleted: {
                        if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                            imageSource = "asset:///images/sound_dk.png"
                        }
                    }
                }
            }
        }
    }
    
    Container {
        topPadding: 20
        leftPadding: 20
        rightPadding: 20
        
        Container {
            horizontalAlignment: HorizontalAlignment.Center
            preferredWidth: 768
            
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }
            
            Label {
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                preferredWidth: 768
                text: qsTr("Audio Enabled")
            }

            ToggleButton {
                id: sound
                objectName: "sound"
                horizontalAlignment: HorizontalAlignment.Center
                function onAudioEnabledChanged() {
                    checked = _settings.GameAudioSettings.AudioEnabled
                }

                function onChanged(checked) {
                    _settings.GameAudioSettings.AudioEnabled = checked
                }

                onCreationCompleted: {
                    onAudioEnabledChanged()
                    _settings.GameAudioSettings.AudioEnabledChanged.connect(onAudioEnabledChanged)
                    checkedChanged.connect(onChanged)
                }
            }
        }

        Label {
            id: _help
            text: qsTr("Enable or disable the audio plugin for the emulator.")
            multiline: true
            visible: helpEnabled
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontStyle: FontStyle.Italic
            } 
        }
    }
}
