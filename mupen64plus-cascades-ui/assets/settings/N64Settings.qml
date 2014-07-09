import bb.cascades 1.0

Container {
    property bool helpEnabled
    
    SettingsToggle {
        title: qsTr("Frameskip")
        helpText: qsTr("Increase gamespeed with lower framerate.")
        showHelp: helpEnabled
        
        function onFrameskipChanged() {
            checked = _settings.N64Settings.Frameskip
        }
        
        function onChanged() {
            _settings.N64Settings.Frameskip = checked
        }
        
        onCreationCompleted: {
            onFrameskipChanged()
            _settings.N64Settings.FrameskipChanged.connect(onFrameskipChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Fog")
        helpText: qsTr("Enable the rendering of fog. May cause issues.")
        showHelp: helpEnabled
        
        function onFogChanged() {
            checked = _settings.N64Settings.Fog
        }
        
        function onChanged() {
            _settings.N64Settings.Fog = checked
        }
        
        onCreationCompleted: {
            onFogChanged()
            _settings.N64Settings.FogChanged.connect(onFogChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("2x Sai Texture Filter")
        helpText: qsTr("Enable the 2x scale and interpolation engine.")
        showHelp: helpEnabled
        
        function onSaiTextureFilterChanged() {
            checked = _settings.N64Settings.SaiTextureFilter
        }
        
        function onChanged() {
            _settings.N64Settings.SaiTextureFilter = checked
        }
        
        onCreationCompleted: {
            onSaiTextureFilterChanged()
            _settings.N64Settings.SaiTextureFilterChanged.connect(onSaiTextureFilterChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Force Screen Clear")
        helpText: qsTr("")
        showHelp: helpEnabled
        
        function onForceScreenClearChanged() {
            checked = _settings.N64Settings.ForceScreenClear
        }
        
        function onChanged() {
            _settings.N64Settings.ForceScreenClear = checked
        }
        
        onCreationCompleted: {
            onForceScreenClearChanged()
            _settings.N64Settings.ForceScreenClearChanged.connect(onForceScreenClearChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Alpha Test")
        helpText: qsTr("Enable alpha tests.")
        showHelp: helpEnabled
        
        function onAlphaTestChanged() {
            checked = _settings.N64Settings.AlphaTest
        }
        
        function onChanged() {
            _settings.N64Settings.AlphaTest = checked
        }
        
        onCreationCompleted: {
            onAlphaTestChanged()
            _settings.N64Settings.AlphaTestChanged.connect(onAlphaTestChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Z Hack")
        helpText: qsTr("Fix issues related to Z-ordering (ie. flickering textures).")
        showHelp: helpEnabled
        
        function onZHackChanged() {
            checked = _settings.N64Settings.ZHack
        }
        
        function onChanged() {
            _settings.N64Settings.ZHack = checked
        }
        
        onCreationCompleted: {
            onZHackChanged()
            _settings.N64Settings.ZHackChanged.connect(onZHackChanged)
            checkedChanged.connect(onChanged)
        }
    }
}
