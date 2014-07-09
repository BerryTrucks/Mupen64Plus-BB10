import bb.cascades 1.0

Sheet {
    id: playSheet
    peekEnabled: false
    
    signal closing()
    
    Page {
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            Container {
                preferredHeight: _frontend.emuHeight
                preferredWidth: _frontend.emuWidth
                translationX: _frontend.rotateQ10 == 0 ? 0 : (_frontend.rotateQ10 < 0 ? _frontend.menuOffset : -_frontend.menuOffset)
                translationY: _frontend.rotateQ10 == 0 ? _frontend.menuOffset : 0
                ForeignWindowControl {
                    id: mainWindow
                    objectName: "myForeignWindow"
                    keyInputForwardingEnabled: true
                    windowId: "emulator_m64p"
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    updatedProperties: WindowProperty.Size | WindowProperty.Position | WindowProperty.Visible
                    visible: _frontend.UseForeignWindowControl
                    
                    function onFocusForeignWindow() {
                        requestFocus()
                    }
                    
                    onCreationCompleted: {
                        _frontend.FocusForeignWindow.connect(onFocusForeignWindow)
                    }
                }
            }
            Container {
                rotationZ: _frontend.rotateQ10 == 0 ? 0 : (_frontend.rotateQ10 < 0 ? -90 : 90)
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
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
                        
                        function onSaveStateSlotChanged() {
                            imageSource = "asset:///images/save_load" + _frontend.SaveStateSlot + ".png"
                        }
    
                        onClicked: {
                            _frontend.SaveState()
                        }
    
                        onLongTouch: {
                             _frontend.selectState(true)
                         }
                        
                        onCreationCompleted: {
                            onSaveStateSlotChanged()
                            _frontend.SaveStateSlotChanged.connect(onSaveStateSlotChanged)
                        }
                    }
                    MupenMenuItem {
                        imageSource: "asset:///images/overlay.png"
                        textUpper: qsTr("Cycle")
                        textLower: qsTr("Overlay")
                        visible: !_frontend.Keyboard && !_frontend.noTouchScreenControllers
    
                        onClicked: {
                            _frontend.LoadTouchOverlay()
                        }
                    }
                    /*MupenMenuItem {
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
                    }*/
                    MupenMenuItem {
                        imageSource: "asset:///images/gameshark.png"
                        textUpper: qsTr("GameShark")
                        textLower: qsTr("Button")
                        
                        onClicked: {
                            _frontend.pressGameshark()
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
                        
                        function onSaveStateSlotChanged() {
                            imageSource = "asset:///images/save_load" + _frontend.SaveStateSlot + ".png"
                        }
                        
                        onClicked: {
                            _frontend.LoadState()
                        }
                        
                        onLongTouch: {
                            _frontend.selectState(false)
                        }
                        
                        onCreationCompleted: {
                            onSaveStateSlotChanged()
                            _frontend.SaveStateSlotChanged.connect(onSaveStateSlotChanged)
                        }
                    }
                }
            }
        }
    }
}