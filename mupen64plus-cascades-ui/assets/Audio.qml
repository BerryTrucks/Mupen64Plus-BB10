import bb.cascades 1.0

Page {
    property bool helpEnabled: false
    property alias sound: sound.checked
    
    actions: [
        ActionItem {
            
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
    
    titleBar: TitleBar {
        id: titleBar4
        title: "Audio Settings"
        visibility:  ChromeVisibility.Visible
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
                text: "Audio Plugin"
            }
            
            ToggleButton {
		        id: sound
		        checked: _frontend.getValueFor(_frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1) + objectName, "true")
		        objectName: "sound"
		        horizontalAlignment: HorizontalAlignment.Center
		        onCheckedChanged: {
		            _frontend.saveValueFor(_frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1) + sound.objectName, checked)
		        }
		    }
            
        }
        
        Label {
            id: _help
            text: "Enable or disable the audio plugin for the emulator."
            multiline: true
            visible: helpEnabled
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontStyle: FontStyle.Italic
            } 
        }
        
        Divider {}
	}
}