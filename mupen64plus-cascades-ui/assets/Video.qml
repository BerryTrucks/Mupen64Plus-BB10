import bb.cascades 1.0

Page {
    id: video
    property alias videoPlugin: videoPlugin.selectedIndex
    titleBar: TitleBar {
        id: titleBar2
        title: "Video Plugin Settings"
        visibility:  ChromeVisibility.Visible
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
	        background: Color.create("#E8E8E8");
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
		    }
		}
	    
	    Container {
	        topPadding: 10
	        leftPadding: 20
	        rightPadding: 20
	        
	        ScrollView {
	            preferredHeight: 1280
	            Container{
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
	        	                id: videoPlugin
	        	                objectName: "videoPlugin"
	        	                horizontalAlignment: HorizontalAlignment.Right
	        	                title: "Video Plugin"
	        	                
	        	
	        	                // Button 1
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
	        	
	        	                // Button 2
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
	        	            }
	        	        }
	        	        Divider {}
	                }
	                
			        Container{
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
				}
			}
	    }
	}
}