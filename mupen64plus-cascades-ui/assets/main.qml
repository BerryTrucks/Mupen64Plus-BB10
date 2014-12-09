/* Copyright (c) 2012 Research In Motion Limited.
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import bb.cascades 1.0
import bb.cascades.pickers 1.0
import "settings/"

TabbedPane {
    id: mainPane
    //showTabsOnActionBar: true
    Tab {
        title: qsTr("Home")
        imageSource: "asset:///images/home.png"
        MainMenu {
        }
    }
    Tab {
        title: qsTr("Hardware")
	    id: generalTest
	    imageSource: "asset:///images/hardware.png"
	    HardwareSettings {
	        id: hardware
	    }
    }
    Tab {
        title: qsTr("Video")
        imageSource: "asset:///images/video.png"
        VideoSettings {
            id: video
        }
    }
    Tab {
        title: qsTr("Audio")
        imageSource: "asset:///images/sound.png"
        AudioSettings {
            id: audio
        }
    }
    Tab {
        title: qsTr("Input")
        imageSource: "asset:///images/controller.png"
        InputSettings {}
    }
    //This tab is dynamically generated in C++
    Tab {
        title: qsTr("Cheats")
        imageSource: "asset:///images/ic_lock.png"
        //Cheats {}
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
                            text: qsTr("Cheat Codes")
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
                            imageSource: "asset:///images/ic_lock.png"
                            
                            onCreationCompleted: {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    imageSource = "asset:///images/ic_lock_dk.png"
                                }
                            }
                        }
                    }
                }
            }
            ScrollView {
                preferredWidth: _frontend.width
                preferredHeight: _frontend.height
                
		        Container {
                    objectName: "cheats"
		            
		            Container {
		                preferredWidth: _frontend.width
		                preferredHeight: _frontend.height
		            
			            layout: DockLayout {}
	            
			            Label {
			                verticalAlignment: VerticalAlignment.Center
			                horizontalAlignment: HorizontalAlignment.Center
			                text: qsTr("Select a ROM") 
			                textStyle {
					            base: SystemDefaults.TextStyles.BigText
					        }  
			            }
		            }
		        }
	        }
	    }
    }

    Menu.definition: MenuDefinition {
        helpAction: HelpActionItem {
            title: qsTr("About")
            imageSource: "asset:///images/ic_info.png"
             
            onTriggered: {
                var sheet = aboutSheet.createObject(_frontend)
                sheet.open()
            }
        }
        actions: [
            ActionItem {
                title: qsTr("Backup")
                imageSource: "asset:///images/backuprestore.png"
                
                onTriggered: {
                    var sheet = backrestSheet.createObject(_frontend)
                    sheet.open()
                }
            }
        ]
        settingsAction: SettingsActionItem {
            title: qsTr("Settings")
            
            onTriggered: {
                var sheet = settingsSheet.createObject(_frontend)
                sheet.open()
            }
        }
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: aboutSheet
            source: "about.qml"
        },
        Tab {
            id: hdmiTab
            title: qsTr("HDMI")
            imageSource: "asset:///images/ic_hdmi.png"
            
            Hdmi {
            }
        },
        ComponentDefinition {
            id: backrestSheet
            source: "backrest.qml"
        },
        ComponentDefinition {
            id: settingsSheet
            source: "settings/GeneralSettings.qml"
        }
    ]
    
    function hdmiDetected(attached) {
        if (attached) {
            mainPane.add(hdmiTab)
        }
        else {
            mainPane.remove(hdmiTab)
        }
    }
    
    onCreationCompleted: {
        _frontend.hdmiDetected.connect(hdmiDetected)
    }
}
