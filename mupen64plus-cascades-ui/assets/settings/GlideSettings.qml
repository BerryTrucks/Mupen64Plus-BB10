import bb.cascades 1.0

Container {
    property bool helpEnabled: false
    
    Container {
        leftPadding: 15
        topPadding: 15
        rightPadding: 15
        horizontalAlignment: HorizontalAlignment.Fill

        Button {
            text: qsTr("Compatibility List")
            horizontalAlignment: HorizontalAlignment.Fill
            
            onClicked: {
                var sheet = compatSheet.createObject(_frontend)
                sheet.open()
            }
        }
    }
    
    Container {
        leftPadding: 15
        rightPadding: 15
        Label {
            id: help
            visible: helpEnabled
            text: qsTr("A list of compatible games and their best settings. Just for use as a guideline as it was not made for this platform.")
            multiline: true
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontStyle: FontStyle.Italic
            } 
        }
    }
    
    Container {
        topPadding: 20.0
        bottomPadding: 5.0
        Divider {
        }
    }
    
    SettingsToggle {
        title: qsTr("Read Every Frame")
        helpText: qsTr("Read framebuffer every frame (may be slow use only for effects that need it e.g. Banjo Kazooie, DK64 transitions).")
        showHelp: helpEnabled
        
        function onReadEveryFrameChanged() {
            checked = _settings.GlideSettings.ReadEveryFrame
        }
        
        function onChanged() {
            _settings.GlideSettings.ReadEveryFrame = checked
        }
        
        onCreationCompleted: {
            onReadEveryFrameChanged()
            _settings.GlideSettings.ReadEveryFrameChanged.connect(onReadEveryFrameChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    Container {
        topPadding: 15
        leftPadding: 15
        rightPadding: 15
        DropDown {
            title: qsTr("Display Stats")
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            
            Option {
                text: qsTr("None")
                value: 0
            }
            Option {
                text: qsTr("FPS Counter")
                value: 1
            }
            Option {
                text: qsTr("VI/s Counter")
                value: 2
            }
            Option {
                text: qsTr("% Speed")
                value: 4
            }
            Option {
                text: qsTr("FPS Transparent")
                value: 8
            }
            Option {
                text: qsTr("FPS and VI/s Counter")
                value: 3
            }
            Option {
                text: qsTr("FPS Counter and % Speed")
                value: 5
            }
            Option {
                text: qsTr("VI/s Counter and % Speed")
                value: 6
            }
            Option {
                text: qsTr("FPS and VI/s Counter and % Speed")
                value: 7
            }
            
            function onDisplayStatsChanged() {
                var val = _settings.GlideSettings.DisplayStats
                if (val == 0) {
                    selectedIndex = 0
                }
                else if (val == 1) {
                    selectedIndex = 1
                }
                else if (val == 2) {
                    selectedIndex = 2
                }
                else if (val == 4) {
                    selectedIndex = 3
                }
                else if (val == 8) {
                    selectedIndex = 4
                }
                else if (val == 3) {
                    selectedIndex = 5
                }
                else if (val == 5) {
                    selectedIndex = 6
                }
                else if (val == 6) {
                    selectedIndex = 7
                }
                else if (val == 7) {
                    selectedIndex = 8
                }
            }
            
            function onChanged(selectedIndex) {
                _settings.GlideSettings.DisplayStats = selectedOption.value
            }
            
            onCreationCompleted: {
                onDisplayStatsChanged()
                _settings.GlideSettings.DisplayStatsChanged.connect(onDisplayStatsChanged)
                selectedIndexChanged.connect(onChanged)
            }
        }
        
        Label {
            multiline: true
            visible: helpEnabled
            text: qsTr("Display performance statistics")
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontStyle: FontStyle.Italic
            }
        }
    }
    
    Container {
        topPadding: 20.0
        bottomPadding: 5.0
        Divider {
        }
    }
    
    SettingsToggle {
        id: displayTime
        title: qsTr("Display Time")
        helpText: qsTr("Display the current time.")
        showHelp: helpEnabled
        
        function onDisplayTimeChanged() {
            checked = _settings.GlideSettings.DisplayTime
        }
        
        function onChanged() {
            _settings.GlideSettings.DisplayTime = checked
        }
        
        onCreationCompleted: {
            onDisplayTimeChanged()
            _settings.GlideSettings.DisplayTimeChanged.connect(onDisplayTimeChanged)
            checkedChanged.connect(onChanged)
        }
    }
    
    SettingsToggle {
        title: qsTr("Display Time 24hr")
        helpText: qsTr("Display the time in 24hr format.")
        showHelp: helpEnabled
        visible: displayTime.checked
        
        function onDisplayTime24Changed() {
            checked = _settings.GlideSettings.DisplayTime24
        }
        
        function onChanged() {
            _settings.GlideSettings.DisplayTime24 = checked
        }
        
        onCreationCompleted: {
            onDisplayTime24Changed()
            _settings.GlideSettings.DisplayTime24Changed.connect(onDisplayTime24Changed)
            checkedChanged.connect(onChanged)
        }
    }
    
    Container {
        topPadding: 15
        leftPadding: 15
        rightPadding: 15
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
            
            function onHiResModeChanged() {
                selectedIndex = _settings.GlideSettings.HiResMode + 1
            }
            
            function onChanged(selectedIndex) {
                _settings.GlideSettings.HiResMode = selectedIndex - 1
            }
            
            onCreationCompleted: {
                onHiResModeChanged()
                _settings.GlideSettings.HiResModeChanged.connect(onHiResModeChanged)
                selectedIndexChanged.connect(onChanged)
            }
        }
        
        Label {
            multiline: true
            visible: helpEnabled
            text: qsTr("Hardware frame buffer emulation")
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontStyle: FontStyle.Italic
            }
        }
    }
    
    Container {
        topPadding: 20.0
        bottomPadding: 5.0
        Divider {
        }
    }
    
    Container {
        topPadding: 15
        leftPadding: 15
        rightPadding: 15
        DropDown {
            title: qsTr("Aspect Ratio")
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            
            Option {
                text: qsTr("Game Default")
            }
            Option {
                text: qsTr("4:3")
            }
            Option {
                text: qsTr("16:9")
            }
            Option {
                text: qsTr("Stretch")
            }
            Option {
                text: qsTr("Original")
            }
            
            function onAspectRatioChanged() {
                selectedIndex = _settings.GlideSettings.AspectRatio + 1
            }
            
            function onChanged(selectedIndex) {
                _settings.GlideSettings.AspectRatio = selectedIndex - 1
            }
            
            onCreationCompleted: {
                onAspectRatioChanged()
                _settings.GlideSettings.AspectRatioChanged.connect(onAspectRatioChanged)
                selectedIndexChanged.connect(onChanged)
            }
        }
        
        Label {
            multiline: true
            visible: helpEnabled
            text: qsTr("Set the displays aspect ratio.")
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontStyle: FontStyle.Italic
            }
        }
    }
    
    Container {
        topPadding: 20.0
        bottomPadding: 5.0
        Divider {
        }
    }
    
    SettingsToggle {
        id: autoFrameskip
        title: qsTr("Auto Frameskip")
        helpText: qsTr("Increase gamespeed with lower framerate.")
        showHelp: helpEnabled
        
        function onFrameskipChanged() {
            checked = _settings.GlideSettings.AutoFrameskip
        }
        
        function onChanged() {
            _settings.GlideSettings.AutoFrameskip = checked
        }
        
        onCreationCompleted: {
            onFrameskipChanged()
            _settings.GlideSettings.AutoFrameskipChanged.connect(onFrameskipChanged)
            checkedChanged.connect(onChanged)
        }
    }

    Container {
        leftPadding: 15.0
        rightPadding: 15.0
        topPadding: 20.0
        visible: !autoFrameskip.checked
        DropDown {
            title: qsTr("Max Frameskip")
            Option {
                text: qsTr("Never skip frames")
            }
            Option {
                text: qsTr("No more than 1 frame")
            }
            Option {
                text: qsTr("No more than 2 frames")
            }
            Option {
                text: qsTr("No more than 3 frames")
            }
            Option {
                text: qsTr("No more than 4 frames")
            }
            Option {
                text: qsTr("No more than 5 frames")
            }
            Option {
                text: qsTr("Exactly 1 frame")
            }
            Option {
                text: qsTr("Exactly 2 frames")
            }
            Option {
                text: qsTr("Exactly 3 frames")
            }
            Option {
                text: qsTr("Exactly 4 frames")
            }
            Option {
                text: qsTr("Exactly 5 frames")
            }
            
            function onMaxFrameskipChanged() {
                selectedIndex = _settings.GlideSettings.MaxFrameskip
            }
            
            function onChanged(selectedIndex) {
                _settings.GlideSettings.MaxFrameskip = selectedIndex
            }
            
            onCreationCompleted: {
                onMaxFrameskipChanged()
                _settings.GlideSettings.MaxFrameskipChanged.connect(onMaxFrameskipChanged)
                selectedIndexChanged.connect(onChanged)
            }
        }
    }
    
    Divider {
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: compatSheet
            source: "../glidecompat.qml"
        }
    ]
}
