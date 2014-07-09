import bb.cascades 1.0

Container {
    property alias button: _button.text
    property int boundKey: -1
    bottomPadding: 20
    
    horizontalAlignment: HorizontalAlignment.Fill
    
    Container {
        leftPadding: 15
        rightPadding: 15
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        
        Label {
            horizontalAlignment: HorizontalAlignment.Left
            verticalAlignment: VerticalAlignment.Center
            preferredWidth: 768
            id: _button
        }
        
        Button {
            id: _mapping
            horizontalAlignment: HorizontalAlignment.Right
            verticalAlignment: VerticalAlignment.Center
            minWidth: 250
            text: _frontend.getMogaInputCharacter(boundKey)

            onClicked: {
                var mapButton = _frontend.mapButton();
                if(mapButton != -1){
                    boundKey = mapButton
                    _mapping.text = _frontend.getMogaInputCharacter(mapButton)
                }
            }
        }
    }
    
    Divider {}
    
    onBoundKeyChanged: {
        _mapping.text = _frontend.getMogaInputCharacter(boundKey)
    }
}