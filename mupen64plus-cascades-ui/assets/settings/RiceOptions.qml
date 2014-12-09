import bb.cascades 1.0

Container {
    id: optionRoot
    property bool helpEnabled
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        visible: advancedOption.added
        maxHeight: advancedOption.added ? 4000 : 0
        layout: DockLayout {
        }
        SegmentedControl {
            horizontalAlignment: HorizontalAlignment.Fill
            id: optionTypeSelector
            Option {
                text: qsTr("Normal")
            }
        }
    }
    
    RiceSettings {
        visible: optionTypeSelector.selectedIndex == 0
        helpEnabled: optionRoot.helpEnabled
    }
    
    RiceINISettings {
        visible: optionTypeSelector.selectedIndex == 1
    }
    
    attachedObjects: [
        Option {
            id: advancedOption
            property bool added: false
            text: qsTr("Advanced")
        }
    ]
    
    function onRomLoaded() {
        if (!advancedOption.added) {
            advancedOption.added = true
            optionTypeSelector.add(advancedOption)
        }
    }
    
    onCreationCompleted: {
        _frontend.ROMLoaded.connect(onRomLoaded)
    }
}
