import bb.cascades 1.0

Container {
    id: root
    property alias help: help.text
    property string settingName
    property alias title: title.text
    property alias settingEnabled: toggle.checked
    property alias showHelp: help.visible 
    property string plugin
    property string defaultValue
    
    bottomPadding: 20
    
    function getPluginValue(_plugin, value){
        if(_plugin == "Video-Rice"){
            return value;
        } else {
            if(value == "True") {
                return "1";
            } else {
                return "0";
            }
        }
    }
    
    function setDefault(){
        toggle.checked = (getPluginValue(plugin,defaultValue) == getPluginValue(plugin, "True"));
    }
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        
        Label {
            id: title
            preferredWidth: 768
            verticalAlignment: VerticalAlignment.Center
        }
        
        ToggleButton {
            id: toggle
            checked: (_frontend.getConfigValue(_frontend.rom, plugin, settingName, root.getPluginValue(plugin,defaultValue)) == root.getPluginValue(plugin,"True"))
            
            onCheckedChanged: {
                if(checked)
                    _frontend.saveConfigValue(plugin, settingName, root.getPluginValue(plugin,"True"));
                else
                    _frontend.saveConfigValue(plugin, settingName, root.getPluginValue(plugin,"False"));
            }
        }
    }
    
    Label {
        id: help
        multiline: true
        textStyle {
            base: SystemDefaults.TextStyles.BodyText
            fontStyle: FontStyle.Italic
        } 
    }
    
    Divider{}
}