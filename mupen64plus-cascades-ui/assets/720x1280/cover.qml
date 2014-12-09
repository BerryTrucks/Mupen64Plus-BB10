import bb.cascades 1.0

Container {
    maxWidth: 319.0
    maxHeight: 437.0
    background: Color.Black
    layout: DockLayout {
    }
    
    ImageView {
        imageSource: "asset:///images/mupen64plus.png"
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        scalingMethod: ScalingMethod.AspectFill
        minWidth: 300
        minHeight: 300
        
        function updateImage() {
            resetImageSource()
            imageSource = _frontend.coverImage
        }
        
        onCreationCompleted: {
            _frontend.coverImageChanged.connect(updateImage)
        }
    }
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        verticalAlignment: VerticalAlignment.Bottom
        background: Color.create("#8b000000")
        visible: _frontend.currentROM.length > 0
        Label {
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Bottom
            textStyle {
                color: Color.White
                fontSize: FontSize.Small
            }
            text: _frontend.currentROM
            multiline: true
        }
    }
}
