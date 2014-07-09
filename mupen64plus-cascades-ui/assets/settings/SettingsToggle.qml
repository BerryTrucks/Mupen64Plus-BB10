import bb.cascades 1.0

Container {
    property alias title: titlelabel.text
    property alias showHelp: help.visible
    property alias helpText: help.text
    property alias checked: toggle.checked
    
    topPadding: 15
    
    Container {
        layout: DockLayout {
        }
        leftPadding: 15
        rightPadding: 15
        horizontalAlignment: HorizontalAlignment.Fill
        
        Label {
            id: titlelabel
            verticalAlignment: VerticalAlignment.Center
        }
        
        ToggleButton {
            id: toggle
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Right
        }
    }
    
    Container {
        leftPadding: 15
        rightPadding: 15
        Label {
            id: help
            multiline: true
            textStyle {
                base: SystemDefaults.TextStyles.BodyText
                fontStyle: FontStyle.Italic
            } 
        }
    }
    
    Divider{}
}