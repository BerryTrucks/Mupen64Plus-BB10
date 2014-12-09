import bb.cascades 1.0

Sheet {
    id: root
    property Container currentEdit: analog
    property alias actionBarVisibility: page.actionBarVisibility
    property bool creating: false
    
    peekEnabled: false
    
    Page {
        id: page
        actionBarVisibility: ChromeVisibility.Overlay
        
        Container {
            layout: DockLayout {
            }
            verticalAlignment: VerticalAlignment.Fill
            horizontalAlignment: HorizontalAlignment.Fill
            background: Color.Gray
            Container {
                id: placementArea
                property int touchX: 0
                property int touchY: 0
                property int startX: 0
                property int startY: 0
                
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                background: Color.White
                layout: AbsoluteLayout {
                }
                maxHeight: 720.0
                minHeight: 720.0
                preferredHeight: 720.0
                
                Container {
                    id: ltrigger
                    layoutProperties: AbsoluteLayoutProperties {
                        positionY: 10.0
                    }
                    ImageView {
                        imageSource: "asset:///images/overlay/l.png"
                    }
                }
                Container {
                    id: rtrigger
                    layoutProperties: AbsoluteLayoutProperties {
                        positionY: 10.0
                        positionX: 1195.0
                    }
                    ImageView {
                        imageSource: "asset:///images/overlay/r.png"
                    }
                }
                Container {
                    id: start
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 606.0
                        positionY: 610.0
                    }
                    ImageView {
                        imageSource: "asset:///images/overlay/start.png"
                    }
                }
                Container {
                    id: analog
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 60.0
                        positionY: 125.0
                    }
                    ImageView {
                        imageSource: "asset:///images/overlay/analog.png"
                    }
                }
                Container {
                    id: dpad
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 60.0
                        positionY: 420.0
                    }
                    ImageView {
                        imageSource: "asset:///images/overlay/dpad.png"
                    }
                }
                Container {
                    id: cpad
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 1000.0
                        positionY: 390.0
                    }
                    ImageView {
                        imageSource: "asset:///images/overlay/cpad.png"
                    }
                }
                Container {
                    id: abutton
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 1150.0
                        positionY: 150.0
                    }
                    ImageView {
                        imageSource: "asset:///images/overlay/a.png"
                    }
                }
                Container {
                    id: bbutton
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 1060.0
                        positionY: 100.0
                    }
                    ImageView {
                        imageSource: "asset:///images/overlay/b.png"
                    }
                }
                Container {
                    id: zbutton
                    layoutProperties: AbsoluteLayoutProperties {
                        positionX: 1060.0
                        positionY: 240.0
                    }
                    ImageView {
                        imageSource: "asset:///images/overlay/z.png"
                    }
                }
                
                onTouch: {
                    var alayout = root.currentEdit.layoutProperties
                    if (event.touchType == TouchType.Down) {
                        touchX = event.localX
                        touchY = event.localY
                        startX = alayout.positionX
                        startY = alayout.positionY
                    
                    }
                    else if (event.touchType == TouchType.Move) {
                        alayout.positionX = startX + (event.localX - touchX)
                        alayout.positionY = startY + (event.localY - touchY)
                    }
                }
            }
            Container {
                horizontalAlignment: HorizontalAlignment.Right
                verticalAlignment: VerticalAlignment.Bottom
                bottomPadding: 10.0
                rightPadding: 10.0
                visible: (root.actionBarVisibility == ChromeVisibility.Hidden) && !creating
                
                ImageView {
                    imageSource: "asset:///images/overlay/overflow.png"
                    
                    onTouch: {
                        if (event.touchType == TouchType.Up) {
                            root.actionBarVisibility = ChromeVisibility.Overlay
                        }
                    }
                }
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Center
                topPadding: 5.0
                Label {
                    id: edittingLabel
                    text: "Now Editing: Analog Stick"
                }
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                background: Color.create("#7d000000")
                visible: creating
                layout: DockLayout {
                }
                ActivityIndicator {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                    running: creating
                    minWidth: 200.0
                    minHeight: 200.0
                }
            }
        }
        
        actions: [
            ActionItem {
                title: qsTr("Cancel")
                ActionBar.placement: ActionBarPlacement.OnBar
                enabled: !creating
                imageSource: "asset:///images/ic_cancel.png"
                
                onTriggered: {
                    OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DeviceNorth
                    root.close()
                }
            },
            ActionItem {
                title: qsTr("Create Overlay")
                ActionBar.placement: _frontend.playPlacement
                enabled: !creating
                imageSource: _frontend.isOSThree ? "asset:///images/overlay/ic_create_big.png" : "asset:///images/overlay/ic_create.png"
                
                onTriggered: {
                    creating = true
                    root.actionBarVisibility = ChromeVisibility.Hidden
                    root.createOverlay()
                }
            },
            ActionItem {
                title: qsTr("Hide Bar")
                ActionBar.placement: ActionBarPlacement.OnBar
                imageSource: "asset:///images/overlay/ic_hide_unhide.png"
                enabled: !creating
                
                onTriggered: {
                    root.actionBarVisibility = ChromeVisibility.Hidden
                }
            },
            ActionItem {
                title: qsTr("")
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/None.png"
                enabled: !creating
            },
            ActionItem {
                id: abuttonoption
                title: "A Button"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/overlay/emptycircle.png"
                enabled: !creating
                
                onTriggered: {
                    edittingLabel.text = "Now Editing: A Button"
                    root.currentEdit = abutton
                    abuttonoption.imageSource = "asset:///images/overlay/checkcircle.png"
                    bbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    cbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    dbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    anbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    zbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    lbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    rbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    sbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                }
            },
            ActionItem {
                id: bbuttonoption
                title: "B Button"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/overlay/emptycircle.png"
                enabled: !creating
                
                onTriggered: {
                    edittingLabel.text = "Now Editing: B Button"
                    root.currentEdit = bbutton
                    abuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    bbuttonoption.imageSource = "asset:///images/overlay/checkcircle.png"
                    cbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    dbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    anbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    zbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    lbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    rbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    sbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                }
            },
            ActionItem {
                id: cbuttonoption
                title: "C Buttons"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/overlay/emptycircle.png"
                enabled: !creating
                
                onTriggered: {
                    edittingLabel.text = "Now Editing: C Buttons"
                    root.currentEdit = cpad
                    abuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    bbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    cbuttonoption.imageSource = "asset:///images/overlay/checkcircle.png"
                    dbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    anbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    zbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    lbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    rbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    sbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                }
            },
            ActionItem {
                id: dbuttonoption
                title: "D-Pad"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/overlay/emptycircle.png"
                enabled: !creating
                
                onTriggered: {
                    edittingLabel.text = "Now Editing: D-Pad"
                    root.currentEdit = dpad
                    abuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    bbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    cbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    dbuttonoption.imageSource = "asset:///images/overlay/checkcircle.png"
                    anbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    zbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    lbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    rbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    sbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                }
            },
            ActionItem {
                id: anbuttonoption
                title: "Analog Stick"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/overlay/checkcircle.png"
                enabled: !creating
                
                onTriggered: {
                    edittingLabel.text = "Now Editing: Analog Stick"
                    root.currentEdit = analog
                    abuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    bbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    cbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    dbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    anbuttonoption.imageSource = "asset:///images/overlay/checkcircle.png"
                    zbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    lbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    rbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    sbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                }
            },
            ActionItem {
                id: zbuttonoption
                title: "Z Trigger"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/overlay/emptycircle.png"
                enabled: !creating
                
                onTriggered: {
                    edittingLabel.text = "Now Editing: Z Trigger"
                    root.currentEdit = zbutton
                    abuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    bbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    cbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    dbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    anbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    zbuttonoption.imageSource = "asset:///images/overlay/checkcircle.png"
                    lbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    rbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    sbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                }
            },
            ActionItem {
                id: lbuttonoption
                title: "Left Trigger"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/overlay/emptycircle.png"
                enabled: !creating
                
                onTriggered: {
                    edittingLabel.text = "Now Editing: Left Trigger"
                    root.currentEdit = ltrigger
                    abuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    bbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    cbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    dbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    anbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    zbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    lbuttonoption.imageSource = "asset:///images/overlay/checkcircle.png"
                    rbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    sbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                }
            },
            ActionItem {
                id: rbuttonoption
                title: "Right Trigger"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/overlay/emptycircle.png"
                enabled: !creating
                
                onTriggered: {
                    edittingLabel.text = "Now Editing: Right Trigger"
                    root.currentEdit = rtrigger
                    abuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    bbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    cbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    dbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    anbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    zbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    lbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    rbuttonoption.imageSource = "asset:///images/overlay/checkcircle.png"
                    sbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                }
            },
            ActionItem {
                id: sbuttonoption
                title: "Start"
                ActionBar.placement: ActionBarPlacement.InOverflow
                imageSource: "asset:///images/overlay/emptycircle.png"
                enabled: !creating
                
                onTriggered: {
                    edittingLabel.text = "Now Editing: Start"
                    root.currentEdit = start
                    abuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    bbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    cbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    dbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    anbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    zbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    lbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    rbuttonoption.imageSource = "asset:///images/overlay/emptycircle.png"
                    sbuttonoption.imageSource = "asset:///images/overlay/checkcircle.png"
                }
            }
        ]
    }
    
    function createOverlay() {
        _frontend.placeA(abutton.layoutProperties.positionX, abutton.layoutProperties.positionY)
        _frontend.placeB(bbutton.layoutProperties.positionX, bbutton.layoutProperties.positionY)
        _frontend.placeZ(zbutton.layoutProperties.positionX, zbutton.layoutProperties.positionY)
        _frontend.placeCPad(cpad.layoutProperties.positionX, cpad.layoutProperties.positionY)
        _frontend.placeStart(start.layoutProperties.positionX, start.layoutProperties.positionY)
        _frontend.placeDPad(dpad.layoutProperties.positionX, dpad.layoutProperties.positionY)
        _frontend.placeAnalog(analog.layoutProperties.positionX, analog.layoutProperties.positionY)
        _frontend.placeLeft(ltrigger.layoutProperties.positionX, ltrigger.layoutProperties.positionY)
        _frontend.placeRight(rtrigger.layoutProperties.positionX, rtrigger.layoutProperties.positionY)
        _frontend.createLayout()
    }
    
    function onOverlayCreationComplete() {
        OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DeviceNorth
        root.close()
    }
    
    onCreationCompleted: {
        _frontend.overlayCreationComplete.connect(onOverlayCreationComplete)
    }
}
