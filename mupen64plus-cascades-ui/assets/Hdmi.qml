import bb.cascades 1.0

Page {
    
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
                    text: qsTr("HDMI Settings")
                    textStyle.fontSize: FontSize.Large
                    textStyle.fontWeight: FontWeight.W500
                    
                    onCreationCompleted: {
                        if (!_frontend.isOSThree && _frontend.themeIndex == 0) {
                            textStyle.color = Color.White
                        }
                    }
                    verticalAlignment: VerticalAlignment.Center
                }
                
                ImageView {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    imageSource: "asset:///images/ic_hdmi.png"
                    
                    onCreationCompleted: {
                        if (_frontend.isOSThree && _frontend.themeIndex == 0) {
                            imageSource = "asset:///images/ic_hdmi_dk.png"
                        }
                    }
                }
            }
        }
    }
    Container {
        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            leftPadding: 5.0
            rightPadding: 5.0
            topPadding: 15.0
            Label {
                id: title
                preferredWidth: 768
                verticalAlignment: VerticalAlignment.Center
                text: qsTr("Use Native Resolution")
            }
            
            ToggleButton {
                id: toggle
                
                onCheckedChanged: {
                    _frontend.hdmi1080 = checked
                }
                onCreationCompleted: {
                    checked = _frontend.hdmi1080
                }
            }
        }
        
        Label {
            multiline: true
            visible: help.helpEnabled
            text: qsTr("Output video in the connected devices native resolution (may be slow)")
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontStyle: FontStyle.Italic
            } 
        }
        
        Divider{}
        Container {
            leftPadding: 15.0
            rightPadding: 15.0
            visible: !toggle.checked
            DropDown {
                title: qsTr("Resolution")
                Option {
                    text: qsTr("256x224")
                }
                Option {
                    text: qsTr("320x240")
                }
                Option {
                    text: qsTr("640x480")
                }
                Option {
                    text: qsTr("720x720")
                }
                Option {
                    text: qsTr("960x720")
                }
                Option {
                    text: qsTr("1280x720")
                }
                Option {
                    text: qsTr("1280x768")
                }
                
                onCreationCompleted: {
                    selectedIndex = _frontend.hdmiResolution
                }
                
                onSelectedIndexChanged: {
                    _frontend.hdmiResolution = selectedIndex
                }
            }
        }
        Divider {
            visible: !toggle.checked
        }
    }
    actions: [
        ActionItem {
            property bool helpEnabled: false
            id: help
            title: {
                if(helpEnabled){
                    "Hide Help"
                } else {
                    "Show Help"
                }
            }
            imageSource: "asset:///images/ic_help.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if(!helpEnabled)
                    helpEnabled = true;
                else
                    helpEnabled = false;
            }
        }
    ]
}
