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

TabbedPane {
    id: mainPane
    //showTabsOnActionBar: true
    Tab {
        title: "Home" 
        imageSource: "asset:///images/home.png"
        MainMenu {
            
            romDirectory: {
                if(general.sdcard == false){
                    ["/accounts/1000/shared/misc/n64/roms"]
                } else {
                    ["/accounts/100/removable/sdcard"]
                }
            }
            videoPlugin: video.videoPlugin
            sound: audio.sound
            boxartEnabled: general.boxartEnabled
        }
    }
    Tab {
	    title: "General"
	    id: generalTest
	    General {
	        id: general
	    }
    }
    Tab {
        title: "Video"
        imageSource: "asset:///images/video.png"
        Video {
            id: video
        }
    }
    Tab {
        title: "Audio"
        imageSource: "asset:///images/sound.png"
        Audio {
            id: audio
        }
    }
    Tab {
        title: "Input"
        imageSource: "asset:///images/input.png"
        Input {}
    }
    //This tab is dynamically generated in C++
    Tab {
        title: "Cheats"
        imageSource: "asset:///images/ic_lock.png"
        //Cheats {}
        Page{
            titleBar: TitleBar {
                id: titleBar
                title: "Cheat Codes"
                visibility:  ChromeVisibility.Visible
                scrollBehavior: TitleBarScrollBehavior.Sticky
            }
            ScrollView{
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
}
