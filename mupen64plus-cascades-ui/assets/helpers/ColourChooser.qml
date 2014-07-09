import bb.cascades 1.0

Container {
    property alias red: primaryRed.value
    property alias green: primaryGreen.value
    property alias blue: primaryBlue.value
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    Container {
        minWidth: 100
        minHeight: 215
        background: Color.create(primaryRed.immediateValue / 255.0, primaryGreen.immediateValue / 255.0, primaryBlue.immediateValue / 255.0)
    }
    Container {
        leftPadding: 15.0
        Slider {
            id: primaryRed
            fromValue: 0
            toValue: 255
        }
        Slider {
            id: primaryGreen
            fromValue: 0
            toValue: 255
        }
        Slider {
            id: primaryBlue
            fromValue: 0
            toValue: 255
        }
    }
}
