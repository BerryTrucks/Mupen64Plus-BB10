import bb.cascades 1.0

Sheet {
    id: playSheet
    peekEnabled: false
    
    signal closing()
    
    Page {
        Container {
            Container {
                preferredHeight: 175.0
                preferredWidth: _frontend.emuWidth
                background: Color.Black
                layout: StackLayout {
                    orientation: LayoutOrientation.LeftToRight
                }
                MupenMenuItem {
	                imageSource: "asset:///images/save_load.png"

                    textUpper: "Save"
                    textLower: "State"

                    onClicked: {
                        _frontend.SaveState()
                    }
                }
                MupenMenuItem {
                    imageSource: "asset:///images/overlay.png"
                    textUpper: qsTr("Toggle")
                    textLower: qsTr("Overlay")

                    onClicked: {
                        _frontend.LoadTouchOverlay()
                    }
                }
                MupenMenuItem {
                    imageSource: "asset:///images/home.png"
                    textUpper: qsTr("Menu")
                    textLower: qsTr("(Broken)")

                    onClicked: {
                        playSheet.close()
                        _frontend.ExitEmulator()
                        OrientationSupport.supportedDisplayOrientation = 
                        SupportedDisplayOrientation.DisplayPortrait;
                        closing()
                    }
                }
                MupenMenuItem {
                    imageSource: "asset:///images/ic_cancel.png"
	                textUpper: qsTr("Close")
	                textLower: qsTr("")
                }
                MupenMenuItem {
                    imageSource: "asset:///images/save_load.png"
	                textUpper: qsTr("Load")
	                textLower: qsTr("State")
                    
                    onClicked: {
                        _frontend.LoadState()
                    }
                }
            }
        }
    }
}