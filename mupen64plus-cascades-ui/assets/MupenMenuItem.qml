import bb.cascades 1.0

Container {
    id: mupenMenuItem
    property bool pressed: false
    property alias imageSource: img.imageSource
    property alias textUpper: txt1.text
    property alias textLower: txt2.text
    signal clicked()
    
    preferredHeight: 140.0
    preferredWidth: _frontend.Keyboard ? 144.0 : 256.0
    verticalAlignment: VerticalAlignment.Fill
    Container {
        preferredHeight: 4.0
        minHeight: 4.0
        maxHeight: 4.0
        background: mupenMenuItem.pressed ? Color.create("#ff00a8df") : Color.Transparent
        minWidth: 86.0
        preferredWidth: 86.0
        maxWidth: 86.0
        horizontalAlignment: HorizontalAlignment.Center
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        ImageView {
            id: img
            minHeight: 81.0
        }
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        Label {
            id: txt1
            textStyle.color: Color.White
            textStyle.fontSize: FontSize.Small
        }
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        Label {
            id: txt2
            textStyle.color: Color.White
            textStyle.fontSize: FontSize.Small
        }
    }
    
    onTouch: {
        if (event.touchType == TouchType.Up) {
            _frontend.swipedown()
            mupenMenuItem.pressed = false
            clicked()
        }
        else if (event.touchType == TouchType.Cancel) {
            mupenMenuItem.pressed = false
        }
        else if (event.touchType == TouchType.Down) {
            mupenMenuItem.pressed = true
        }
    }
}
