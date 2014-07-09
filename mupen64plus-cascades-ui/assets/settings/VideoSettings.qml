import bb.cascades 1.0
import "../helpers"

Page {
    id: vSettingsPage
    property alias helpEnabled: help.helpEnabled
    
    actions: [
        ShowHelpActionItem {
            id: help
        },
        ActionItem {
            title: qsTr("Set Defaults")
            imageSource: "asset:///images/default_icon.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (videoPlugin.selectedIndex == 0) {
                    _settings.RiceSettings.reset()
                }
                else if (videoPlugin.selectedIndex == 1) {
                    _settings.N64Settings.reset()
                }
                else if (videoPlugin.selectedIndex == 2) {
                    _settings.GlideSettings.reset()
                }
            }
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
                    text: qsTr("Video Settings")
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
                    imageSource: "asset:///images/video.png"
                    
                    onCreationCompleted: {
                        if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                            imageSource = "asset:///images/video_dk.png"
                        }
                    }
                }
            }
        }
    }
    
    Container {
        topPadding: 10
        
        ScrollView {
            Container {
                verticalAlignment: VerticalAlignment.Fill
                horizontalAlignment: HorizontalAlignment.Fill
                
                Container {
                    leftPadding: 15
                    rightPadding: 15

                    DropDown {
                        id: videoPlugin
                        title: qsTr("Video Plugin")
                        
                        Option {
                            id: gles2rice
                            objectName: "gles2rice"
                            text: "GLES2rice"
                        }
                        
                        Option {
                            id: gles2n64
                            objectName: "gles2n64"
                            text: "GLES2n64"
                        }
                        
                        Option {
                            id: glide64mk2
                            objectName: "glide64mk2"
                            text: "GLES2glide (Broken)"
                        }
                        
                        function onVideoPluginChanged() {
                            selectedIndex = _settings.VideoPlugin
                        }
                        
                        function onChanged(selectedIndex) {
                            _settings.VideoPlugin = selectedIndex
                        }
                        
                        onCreationCompleted: {
                            onVideoPluginChanged()
                            _settings.VideoPluginChanged.connect(onVideoPluginChanged)
                            selectedIndexChanged.connect(onChanged)
                        }
                    }
                }
                
                Divider { }
                
                RiceSettings {
                    id: riceSettings
                    visible: gles2rice.selected
                    helpEnabled: help.helpEnabled
                }
                
                N64Settings {
                    id: n64Settings
                    visible: gles2n64.selected
                    helpEnabled: help.helpEnabled
                }
                
                GlideSettings {
                    id: glideSettings
                    visible: glide64mk2.selected
                    helpEnabled: help.helpEnabled
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
