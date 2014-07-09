import bb.cascades 1.0


Container {
    leftPadding: 15.0
    topPadding: 15.0
    rightPadding: 15.0
    horizontalAlignment: HorizontalAlignment.Fill
    
    property alias title: titlebox.text
    property alias value: valuebox.text
    
    Container {
        layout: DockLayout {
        }
        horizontalAlignment: HorizontalAlignment.Fill
        Label {
            id: titlebox
        }
        Label {
            id: valuebox
            horizontalAlignment: HorizontalAlignment.Right
        }
    }
}
