import bb.cascades 1.0

Container {
    property bool helpEnabled
    
    SettingsToggle {
        title: qsTr("Auto Frameskip")
        showHelp: helpEnabled
        helpText: qsTr("Improve gamespeed with lower framerates.")
        
        function onFrameskipChanged() {
            checked = _settings.RiceSettings.Frameskip
        }
        
        function onChanged() {
            _settings.RiceSettings.Frameskip = checked
        }
        
        onCreationCompleted: {
            onFrameskipChanged()
            _settings.RiceSettings.FrameskipChanged.connect(onFrameskipChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Fog")
        showHelp: helpEnabled
        helpText: qsTr("Show fog effects.")
        
        function onFogChanged() {
            checked = _settings.RiceSettings.Fog
        }
        
        function onChanged() {
            _settings.RiceSettings.Fog = checked
        }
        
        onCreationCompleted: {
            onFogChanged()
            _settings.RiceSettings.FogChanged.connect(onFogChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Stretch Video")
        showHelp: helpEnabled
        helpText: qsTr("Stretch the video from its native aspect ratio of 4:3 to the device ratio.")
        
        function onStretchVideoChanged() {
            checked = _settings.RiceSettings.StretchVideo
        }
        
        function onChanged() {
            _settings.RiceSettings.StretchVideo = checked
        }
        
        onCreationCompleted: {
            onStretchVideoChanged()
            _settings.RiceSettings.StretchVideoChanged.connect(onStretchVideoChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Fast Texture CRC")
        showHelp: helpEnabled
        helpText: qsTr("Improve gamespeed with lower texture quality.")
        
        function onFastTextureCRCChanged() {
            checked = _settings.RiceSettings.FastTextureCRC
        }
        
        function onChanged() {
            _settings.RiceSettings.FastTextureCRC = checked
        }
        
        onCreationCompleted: {
            onFastTextureCRCChanged()
            _settings.RiceSettings.FastTextureCRCChanged.connect(onFastTextureCRCChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Accurate Texture Mapping")
        showHelp: helpEnabled
        helpText: qsTr("Improve gamespeed with lower texture quality.")
        
        function onAccurateTextureMappingChanged() {
            checked = _settings.RiceSettings.AccurateTextureMapping
        }
        
        function onChanged() {
            _settings.RiceSettings.AccurateTextureMapping = checked
        }
        
        onCreationCompleted: {
            onAccurateTextureMappingChanged()
            _settings.RiceSettings.AccurateTextureMappingChanged.connect(onAccurateTextureMappingChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Double Small Texture Size")
        showHelp: helpEnabled
        helpText: qsTr("Enhance texture quality.")
        checked: _settings.RiceSettings.DoubleSmallTextureSize
        
        function onDoubleSmallTextureSizeChanged() {
            checked = _settings.RiceSettings.DoubleSmallTextureSize
        }
        
        function onChanged() {
            _settings.RiceSettings.DoubleSmallTextureSize = checked
        }
        
        onCreationCompleted: {
            onDoubleSmallTextureSizeChanged()
            _settings.RiceSettings.DoubleSmallTextureSizeChanged.connect(onDoubleSmallTextureSizeChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    Container {
        topPadding: 15
        rightPadding: 15
        leftPadding: 15
        DropDown {
            title: qsTr("Texture Enhancement")
            selectedIndex: _settings.RiceSettings.TextureEnhancement
    
            Option {
                id: noE
                text: qsTr("No Enhancement")
                value: "0"
            }
            Option {
                id: doubleTex
                text: qsTr("2x (Double Texture Size)")
                value: "1"
            }
            Option {
                id: sai2x
                text: qsTr("2xSaI")
                value: "2"
            }
            Option {
                id: hq2x
                text: qsTr("hq2x")
                value: "3"
            }
            Option {
                id: lq2x
                text: qsTr("lq2x")
                value: "4"
            }
            
            function onTextureEnhancementChanged() {
                selectedIndex = _settings.RiceSettings.TextureEnhancement
            }
            
            function onChanged(selectedIndex) {
                _settings.RiceSettings.TextureEnhancement = selectedIndex
            }
            
            onCreationCompleted: {
                onTextureEnhancementChanged()
                _settings.RiceSettings.TextureEnhancementChanged.connect(onTextureEnhancementChanged)
                selectedIndexChanged.connect(onChanged)
            }
        }
    }
    
    Container {
        rightPadding: 15
        leftPadding: 15
        Label {
            text: qsTr("The enhancement algorithm to be applied to textures.")
            multiline: true
            visible: helpEnabled
        }
    }
    
    Divider {
    }
}
