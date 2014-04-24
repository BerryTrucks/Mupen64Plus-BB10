import bb.cascades 1.0

Container {
    maxWidth: _frontend.Keyboard ? 310.0 : 334.0
    maxHeight: _frontend.Keyboard ? 211.0 : 396.0
    background: Color.Black
    layout: DockLayout {
    }
    
    ImageView {
        imageSource: "asset:///images/mupen64plus.png"
        verticalAlignment: VerticalAlignment.Center
        horizontalAlignment: HorizontalAlignment.Center
        scalingMethod: ScalingMethod.AspectFill
        
        function updateImage() {
            resetImageSource()
            imageSource = _frontend.coverImage
        }
        
        onCreationCompleted: {
            _frontend.coverImageChanged.connect(updateImage)
        }
    }
}
