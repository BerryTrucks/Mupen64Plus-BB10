import bb.cascades 1.0

Container {
    property alias button: _button.text
    property int boundKey: -1
    bottomPadding: 20

    horizontalAlignment: HorizontalAlignment.Fill
    
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        leftPadding: 15
        rightPadding: 15
        layout: DockLayout {
        }

        Label {
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Center
            id: _button
        }
        
        Button {
            id: _mapping
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            minWidth: 250
            text: _frontend.getInputCharacter(boundKey)

            onClicked: {
                var mapButton = _frontend.mapButton();
                if(mapButton != -1) {
                    boundKey = mapButton
                    _mapping.text = _frontend.getInputCharacter(mapButton)
                }
            }
        }
    }
    
    Divider {}
    
    onBoundKeyChanged: {
        _mapping.text = _frontend.getInputCharacter(boundKey)
    }
}

