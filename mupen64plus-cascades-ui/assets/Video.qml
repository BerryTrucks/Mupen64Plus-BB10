import bb.cascades 1.0

Page {
    id: video
    property alias videoPlugin: videoPlugin2.selectedIndex
    titleBar: TitleBar {
        id: titleBar2
        title: "Video Plugin Settings"
        visibility:  ChromeVisibility.Visible
        scrollBehavior: TitleBarScrollBehavior.Sticky
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
        },
        ActionItem {
            title: "Set Defaults"
            imageSource: "asset:///images/default_icon.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                skip.setDefault();
                crc.setDefault();
                texmap.setDefault();
                dbl.setDefault();
                gskip.setDefault();
                gfog.setDefault();
                gsai.setDefault();
                gclear.setDefault();
                galpha.setDefault();
                gdepth.setDefault();
                gz.setDefault();
            }
        }
    ]
    
    Container {
	    Container {
	        preferredWidth: 768
	        horizontalAlignment: HorizontalAlignment.Center
	        background: _frontend.colorIndex == 0 ? Color.create("#E8E8E8") : Color.Black;
	        bottomPadding: 10
	        topPadding: 10
	        
		    SegmentedControl {
		        id: segmented1
		        Option {
		            id: general
		            text: "General"
		            value: general
		            selected: true
		        }
		        Option {
		            id: rice
		            text: "GLES2rice"
		            value: "rice"
		        }
		        Option {
		            id: n64
		            text: "GLES2n64"
		            value: "n64"
		        }

		        onCreationCompleted: {
		            if (_frontend.debugMode) {
		                add(glide64)
		            }
		        }
		        
		        attachedObjects: [
		            Option {
		                id: glide64
		                text: "Glide64 mk2"
                        value: "Glide64mk2"
		            }
		        ]
		    }
		}
	    
	    Container {
	        topPadding: 10
	        leftPadding: 20
	        rightPadding: 20
	        
	        ScrollView {
	            preferredHeight: 1280
	            Container {
	                Container {
	                    visible: general.selected
	                    horizontalAlignment: HorizontalAlignment.Center
	                    preferredWidth: 768
	                    
	                    Container {
	                        topPadding: 10
		                    layout: StackLayout {
	                            orientation: LayoutOrientation.LeftToRight
	                        }
	        	            DropDown {
	        	                id: videoPlugin2
	        	                objectName: "videoPlugin"
	        	                horizontalAlignment: HorizontalAlignment.Right
	        	                title: "Video Plugin"

	        	                Option {
	        	                    id: gles2rice
	        	                    objectName: "gles2rice"
	        	                    text: "GLES2rice"
	        	
	        	                    // Call our C++ getValueFor() function for objectName, which connects to the QSettings.
	        	                    selected: _frontend.getValueFor(_frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1) + objectName, "true")
	        	                    onSelectedChanged: {
	        	                        _frontend.saveValueFor(_frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1) + gles2rice.objectName, gles2rice.selected);
	        	                    }
	        	                }

	        	                Option {
	        	                    id: gles2n64
	        	                    objectName: "gles2n64"
	        	                    text: "GLES2n64"

	        	                    // Call our C++ getValueFor() function for objectName, which connects to the QSettings.
	        	                    selected: _frontend.getValueFor(_frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1) + objectName, "false")
	        	                    onSelectedChanged: {
	        	                        _frontend.saveValueFor(_frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1) + gles2n64.objectName, gles2n64.selected);
	        	                    }
                                }
                                
                                onCreationCompleted: {
                                    if (_frontend.debugMode) {
                                        videoPlugin2.add(glide64mk2)
                                    }
                                }
                                
                                attachedObjects: [
                                    Option {
                                        id: glide64mk2
                                        objectName: "glide64mk2"
                                        text: "Glide64mk2"
                                        
                                        // Call our C++ getValueFor() function for objectName, which connects to the QSettings.
                                        selected: _frontend.getValueFor(_frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1) + objectName, "false")
                                        onSelectedChanged: {
                                            _frontend.saveValueFor(_frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1) + glide64mk2.objectName, glide64mk2.selected);
                                        }
                                    }
                                ]
	        	            }
	        	        }
	        	        Divider {}
	                }
	                
			        Container {
			            visible: rice.selected
			            horizontalAlignment: HorizontalAlignment.Center
			            
			            VideoSettingToggle {
			                id: skip
			                settingName: "SkipFrame"
			                title: "Frameskip"
			                help: "Improve gamespeed with lower framerates"
			                showHelp: help.helpEnabled
			                plugin: "Video-Rice"
			                defaultValue: "False"
			            }
			            
			            VideoSettingToggle {
			                id: crc
			                settingName: "FastTextureLoading"
			                title: "Fast Texture CRC"
			                help: "Improve gamespeed with lower texture quality"
			                showHelp: help.helpEnabled
			                plugin: "Video-Rice"
			                defaultValue: "False"
			            }
			            
			            VideoSettingToggle {
			                id: texmap
			                settingName: "AccurateTextureMapping"
			                title: "Accurate Texture Mapping"
			                help: "Improve gamespeed with lower texture quality"
			                showHelp: help.helpEnabled
			                plugin: "Video-Rice"
			                defaultValue: "True"
			            }
			            
			            VideoSettingToggle {
			                id: dbl
			                settingName: "DoubleSizeForSmallTxtrBuf"
			                title: "Double Small Texture Size"
			                help: "Enhance texture quality"
			                showHelp: help.helpEnabled
			                plugin: "Video-Rice"
			                defaultValue: "False"
			            }
			        
				        DropDown {
				            title : "Texture Enhancement"
				            enabled : true
				            selectedOption: {
				                var val = _frontend.getConfigValue(_frontend.rom, "Video-Rice", "TextureEnhancement", "0");
				                if(val == 0) {
				                    noE;
				                } else if (val == 1){
				                    doubleTex;
				                } else if (val == 2){
				                    sai2x;
			                    } else if (val == 3){
			                        hq2x;
			                    } else if (val == 4){
			                        lq2x;
				                }
				            }
				            
				            onSelectedValueChanged: {
				                _frontend.saveConfigValue("Video-Rice", "TextureEnhancement", selectedValue);
				            }
				         
				            // image + text + description
				            Option {
				                id: noE
				                text : "No Enhancement"
				                value : "0"
				            }
				         
				            // text + description
				            Option {
				                id: doubleTex
				                text : "2x (Double Texture Size)"
				                value : "1"
				            }
				            Option {
				                id: sai2x
				                text : "2xSaI"
				                value : "2"
				            }
				            Option {
				                id: hq2x
				                text : "hq2x"
				                value : "3"
				            }
				            Option {
				                id: lq2x
				                text : "lq2x"
				                value : "4"
				            }
				        }
				        
				        Divider{}
			        }
			    
			        Container {
			            visible: n64.selected
			            horizontalAlignment: HorizontalAlignment.Center
			            
			            VideoSettingToggle {
			                id: gskip
			                title: "Frameskip"
			                settingName: "auto frameskip"
			                help: "Increase gamespeed with lower framerate"
			                showHelp: help.helpEnabled
			                plugin: "gles2n64"
			                defaultValue: "False"
			            }
			            
			            VideoSettingToggle {
			                id: gfog
			                title: "Fog"
			                settingName: "enable fog"
			                help: "Enable the rendering of fog. May cause issues."
			                showHelp: help.helpEnabled
			                plugin: "gles2n64"
			                defaultValue: "False"
			            }
			            VideoSettingToggle {
			                id: gsai
			                title: "2x Sai Texture Filter"
			                settingName: "texture 2xSAI"
			                help: "TODO"
			                showHelp: help.helpEnabled
			                plugin: "gles2n64"
			                defaultValue: "False"
			            }
			            VideoSettingToggle {
			                id: gclear
	   		                title: "Force Screen Clear"
	   		                settingName: "force screen clear"
	   		                help: "TODO"
	   		                showHelp: help.helpEnabled
	   		                plugin: "gles2n64"
	   		                defaultValue: "False"
	   		            }
					    VideoSettingToggle {
					        id: galpha
					        title: "Alpha Test"
					        settingName: "enable alpha test"
					        help: "TODO"
					        showHelp: help.helpEnabled
					        plugin: "gles2n64"
					        defaultValue: "True"
					    }
					    VideoSettingToggle {
					        id: gz
					        title: "Z Hack"
					        settingName: "hack z"
					        help: "TODO"
					        showHelp: help.helpEnabled
					        plugin: "gles2n64"
					        defaultValue: "True"
					    }
                    }
                    
                    Container {
                        visible: glide64.selected
                        
                        Button {
                            text: qsTr("Compatibility List")
                            horizontalAlignment: HorizontalAlignment.Fill
                            
                            onClicked: {
                                var sheet = compatSheet.createObject()
                                sheet.open()
                            }
                        }
                        
                        VideoSettingToggle {
                            id: ref
                            title: "Read Every Frame"
                            settingName: "fb_read_always"
                            help: "Read framebuffer every frame (may be slow use only for effects that need it e.g. Banjo Kazooie, DK64 transitions)"
                            showHelp: help.helpEnabled
                            plugin: "Video-Glide64mk2"
                            defaultValue: "False"
                        }
                        
                        Container {
                            DropDown {
                                title: qsTr("Hi-res Mode")
                                horizontalAlignment: HorizontalAlignment.Right
                                verticalAlignment: VerticalAlignment.Center

                                Option {
                                    text: qsTr("Game Default")
                                }
                                Option {
                                    text: qsTr("Disable")
                                }
                                Option {
                                    text: qsTr("Enable")
                                }
                                
                                onCreationCompleted: {
                                    var val = _frontend.getConfigValue(_frontend.rom, "Video-Glide64mk2", "fb_hires", "-1")
                                    if (val == 1) {
                                        selectedIndex = 2
                                    }
                                    else if (val == 0) {
                                        selectedIndex = 1
                                    }
                                    else {
                                        selectedIndex = 0
                                    }
                                }
                                
                                onSelectedIndexChanged: {
                                    _frontend.saveConfigValue("Video-Glide64mk2", "fb_hires", selectedIndex - 1)
                                }
                            }
                            
                            Label {
                                multiline: true
                                visible: help.helpEnabled
                                text: qsTr("Hardware frame buffer emulation")
                                textStyle {
                                    base: SystemDefaults.TextStyles.BodyText
                                    fontStyle: FontStyle.Italic
                                } 
                            }
                            
                            Divider { }
                        }

                        attachedObjects: [
                            ComponentDefinition {
                                id: compatSheet
                                source: "glidecompat.qml"
                            }
                        ]
                    }
				}
			}
	    }
	}
}