import bb.cascades 1.0

Container {
    property int player  
    
    DropDown {
        id: _device
        title: "Input Device"
        
        Option {
            id: _disabled
            text: "Disabled"
            value: 0
        }
        Option {
            id: _keyboardindex
            text: "Keyboard"
            value: -2
        }
        Option {
            id: _gamepadindex
            text: "GamePad"
            value: -4
        }
        
        onSelectedIndexChanged: {
            if(selectedIndex == 0){
                _frontend.setInputValue(player, "present", 0);
                deviceList.selectedIndex = 0
            } else {
                var val = options[selectedIndex].value
                if (val == -3) {
                    _frontend.setInputValue(player, "device", -3);
                    deviceList.selectedIndex = 0
                    _skin.selectedIndex = 3
                } else if (val == -2) {
                    _frontend.setInputValue(player, "device", -2);
                    deviceList.selectedIndex = 0
                    _skin.selectedIndex = 0
                } else if (val == -4) {
                    _frontend.setInputValue(player, "device", -4);
                    _skin.selectedIndex = 0
                } else if (val == -5) {
                    _frontend.setInputValue(player, "device", -5);
                    deviceList.selectedIndex = 0
                    _skin.selectedIndex = 4
                    _frontend.setInputValue(player, "X Axis Left", "")
                    _frontend.setInputValue(player, "X Axis Right", "")
                    _frontend.setInputValue(player, "Y Axis Up", "")
                    _frontend.setInputValue(player, "Y Axis Down", "")
                    _frontend.setInputValue(player, "Up Left", "")
                    _frontend.setInputValue(player, "Up Right", "")
                    _frontend.setInputValue(player, "Down Left", "")
                    _frontend.setInputValue(player, "Down Right", "")
                }
                _frontend.setInputValue(player, "present", 1);
            }
        }
        
        attachedObjects: [
            Option {
                id: _touchkeyboard
                text: "Touchscreen/Keyboard"
                value: -5
            },
            Option {
                id: _touchscreen
                text: "Touchscreen"
                value: -3
            }
        ]
        
        onCreationCompleted: {
            if (_frontend.Keyboard) {
                add(_touchkeyboard)
            }
            else {
                insert(1, _touchscreen)
            }
            var present = _frontend.getInputValue(player, "present")
            if (present == 0) {
                _disabled.selected = true
            }
            else {
                var index = _frontend.getInputValue(player, "device")
                if (index == -2) {
                    _keyboardindex.selected = true
                }
                else if (index == -3) {
                    _touchscreen.selected = true
                }
                else if (index == -4) {
                    _gamepadindex.selected = true
                }
                else if (index == -5) {
                    _touchkeyboard.selected = true
                }
            }
        }
    }
    
    Divider {}
	
	//Touchscreen
	Container {
	    id:_touch
	    visible: _touchscreen.selected

        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight
            }

            Label {
                text: qsTr("Rumble Pak")
                preferredWidth: 768
                verticalAlignment: VerticalAlignment.Center
            }

            ToggleButton {
                id: toggle
                checked:_frontend.getInputValue(player, "plugin") == 5

                onCheckedChanged: {
                    if(checked)
                        _frontend.setInputValue(player, "plugin", 5)
                    else
                        _frontend.setInputValue(player, "plugin", 0)
                }
            }
        }
	    
	    DropDown {
	        id: _skin
	        title: "Controller Layout"
	        
	        Option {
	            text: "None"
	            value: 0
	            selected: _frontend.getInputValue(player, "layout") == 0
	        }
	        
	        Option {
	            text: "Default"
	            value: 1
	            selected: _frontend.getInputValue(player, "layout") == 1
	        }
	        
	        Option {
	            text: "Alternate"
	            value: 2
	            selected: _frontend.getInputValue(player, "layout") == 2
            }
            
            Option {
                text: "Fullscreen"
                value: 3
                selected: _frontend.getInputValue(player, "layout") == 3
            }
	        
	        onSelectedValueChanged: {
	            _frontend.setInputValue(player, "layout", selectedValue)
	        }
	    }
	    
	    //pick starting layout
	    ImageView {
	        maxWidth: 728
	        imageSource: "asset:///images/" + _skin.selectedOption.text + ".png"
	        scalingMethod: ScalingMethod.AspectFit
	    }
	}
	
	//keyboard
	ScrollView {
        visible: _keyboardindex.selected
		Container {
		    id:_keyboard
		    
		    leftPadding: 20
            rightPadding: 20
            
            Container {
                bottomPadding: 20
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Label {
                        text: qsTr("Rumble Pak")
                        preferredWidth: 768
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    ToggleButton {
                        id: toggle2
                        checked:_frontend.getInputValue(player, "plugin") == 5
                        
                        onCheckedChanged: {
                            if(checked)
                                _frontend.setInputValue(player, "plugin", 5)
                            else
                                _frontend.setInputValue(player, "plugin", 0)
                        }
                    }
                }
                
                Divider {
                }
            }
		    
		    ButtonMap {
		        id: dr1
		        button: "DPad R"
		    }
		    ButtonMap {
		        id: dl1
		        button: "DPad L"
		    }
		    ButtonMap {
		        id: dd1
		        button: "DPad D"
		    }
		    ButtonMap {
		        id: du1
		        button: "DPad U"
		    }
		    ButtonMap {
		        id: start1 
		        button: "Start"
		    }
		    ButtonMap {
		        id: z1
		        button: "Z Trig"
		    }
		    ButtonMap {
		        id: b1 
		        button: "B Button"
		    }
		    ButtonMap {
		        id: a1 
		        button: "A Button"
		    }
		    ButtonMap {
		        id: cr1
		        button: "C Button R"
		    }
            ButtonMap {
                id: cl1
		        button: "C Button L"
		    }
            ButtonMap {
                id: cd1
		        button: "C Button D"
		    }
            ButtonMap {
                id: cu1
		        button: "C Button U"
		    }
            ButtonMap {
                id: rt1
		        button: "R Trig"
		    }
            ButtonMap {
                id: lt1
		        button: "L Trig"
		    }
            ButtonMap {
                id: mem1
		        button: "Mempak switch"
		    }
            ButtonMap {
                id: rumble1
		        button: "Rumblepak switch"
		    }
            ButtonMap {
                id: xl1
		        button: "X Axis Left"
		    }
            ButtonMap {
                id: xr1
		        button: "X Axis Right"
		    }
            ButtonMap {
                id: xu1
		        button: "Y Axis Up"
		    }
            ButtonMap {
                id: xd1
		        button: "Y Axis Down"
		    }
            ButtonMap {
                id: xul1
                button: "Up Left"
            }
            ButtonMap {
                id: xur1
                button: "Up Right"
            }
            ButtonMap {
                id: xdl1
                button: "Down Left"
            }
            ButtonMap {
                id: xdr1
                button: "Down Right"
            }
		}
    }
	
	//gamepad
	Container {
	    id:_gamepad
	    
        visible: _gamepadindex.selected
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                DropDown {
                    id: deviceList
                    title: qsTr("Controller")
                    objectName: "deviceList" + player
                    selectedIndex: 1

                    Option {
                        text: qsTr("None")
                        selected: true
                    }

                    onOptionAdded: {
                        selectedIndex = _frontend.getControllerIndex(player)
                    }

                    function setSelectedIndex(selectedIndex) {
                        _frontend.setControllerIndex(player, selectedIndex)
                    }
                    
                    function controllersDetected() {
                        selectedIndexChanged.connect(setSelectedIndex)
                    }
                    
                    onCreationCompleted: {
                        _frontend.controllersDetected.connect(controllersDetected)
                    }
                }
                MogaButtonMap {
                    id: dr2
                    button: "DPad R"
                }
                MogaButtonMap {
                    id: dl2
                    button: "DPad L"
                }
                MogaButtonMap {
                    id: dd2
                    button: "DPad D"
                }
                MogaButtonMap {
                    id: du2
                    button: "DPad U"
                }
                MogaButtonMap {
                    id: start2 
                    button: "Start" 
                }
                MogaButtonMap {
                    id: z2
                    button: "Z Trig"
                }
                MogaButtonMap {
                    id: b2 
                    button: "B Button"
                }
                MogaButtonMap {
                    id: a2 
                    button: "A Button"
                }
                MogaButtonMap {
                    id: cr2
                    button: "C Button R"
                }
                MogaButtonMap {
                    id: cl2
                    button: "C Button L"
                }
                MogaButtonMap {
                    id: cd2
                    button: "C Button D"
                }
                MogaButtonMap {
                    id: cu2
                    button: "C Button U"
                }
                MogaButtonMap {
                    id: rt2
                    button: "R Trig"
                }
                MogaButtonMap {
                    id: lt2
                    button: "L Trig"
                }
                MogaButtonMap {
                    id: mem2
                    button: "Mempak switch"
                }
                MogaButtonMap {
                    id: rumble2
                    button: "Rumblepak switch"
                }
                DropDown {
                    id: _analogType
                    title: qsTr("Analog Stick Type")
                    
                    options: [
                        Option {
                            text: qsTr("Keys")
                        },
                        Option {
                            text: qsTr("Analog0")
                        },
                        Option {
                            text: qsTr("Analog1")
                        }
                    ]
                    
                    onCreationCompleted: {
                        var i = _frontend.getInputValue(player, "X Axis Left")
                        if (i == -2) {
                            selectedIndex = 1
                        }
                        else if (i == -3) {
                            selectedIndex = 2
                        }
                        else {
                            selectedIndex = 0
                        }
                        selectedIndexChanged.connect(selectionChanged)
                    }
                    
                    function selectionChanged(index) {
                        if (index == 1) {
                            _frontend.setInputValue(player,"X Axis Left",-2)
                        }
                        else if (index == 2) {
                            _frontend.setInputValue(player,"X Axis Left",-3)
                        }
                        else {
                            _frontend.setInputValue(player,"X Axis Left",-1)
                        }
                    }
                }
                Divider {
                }
                MogaButtonMap {
                    id: xl2
                    visible: _analogType.selectedIndex == 0
                    button: "X Axis Left"
                }
                MogaButtonMap {
                    id: xr2
                    visible: _analogType.selectedIndex == 0
                    button: "X Axis Right"
                }
                MogaButtonMap {
                    id: xu2
                    visible: _analogType.selectedIndex == 0
                    button: "Y Axis Up"
                }
                MogaButtonMap {
                    id: xd2
                    visible: _analogType.selectedIndex == 0
                    button: "Y Axis Down"
                }
            }
        }
    }
    
    //keyboard
    ScrollView {
        visible: _touchkeyboard.selected
        Container {
            id:_keyboardtouch
            
            leftPadding: 20
            rightPadding: 20
            
            Container {
                bottomPadding: 20
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    
                    Label {
                        text: qsTr("Rumble Pak")
                        preferredWidth: 768
                        verticalAlignment: VerticalAlignment.Center
                    }
                    
                    ToggleButton {
                        id: toggle3
                        checked:_frontend.getInputValue(player, "plugin") == 5
                        
                        onCheckedChanged: {
                            if(checked)
                                _frontend.setInputValue(player, "plugin", 5)
                            else
                                _frontend.setInputValue(player, "plugin", 0)
                        }
                    }
                }
                
                Divider {
                }
            }
            
            ButtonMap {
                id: kt_dr1
                button: "DPad R"
            }
            ButtonMap {
                id: kt_dl1
                button: "DPad L"
            }
            ButtonMap {
                id: kt_dd1
                button: "DPad D"
            }
            ButtonMap {
                id: kt_du1
                button: "DPad U"
            }
            ButtonMap {
                id: kt_z1
                button: "Z Trig"
            }
            ButtonMap {
                id: kt_b1 
                button: "B Button"
            }
            ButtonMap {
                id: kt_a1 
                button: "A Button"
            }
            ButtonMap {
                id: kt_cr1
                button: "C Button R"
            }
            ButtonMap {
                id: kt_cl1
                button: "C Button L"
            }
            ButtonMap {
                id: kt_cd1
                button: "C Button D"
            }
            ButtonMap {
                id: kt_cu1
                button: "C Button U"
            }
            ButtonMap {
                id: kt_rt1
                button: "R Trig"
            }
            ButtonMap {
                id: kt_lt1
                button: "L Trig"
            }
            ButtonMap {
                id: kt_mem1
                button: "Mempak switch"
            }
            ButtonMap {
                id: kt_rumble1
                button: "Rumblepak switch"
            }
        }
    }
	
    function reset() {
        _device.selectedIndex = 1
        _skin.selectedIndex = 1
        dr1.reset()
        dl1.reset()
        dd1.reset()
        du1.reset()
        start1.reset()
        z1.reset()
        b1.reset()
        a1.reset()
        cr1.reset()
        cl1.reset()
        cd1.reset()
        cu1.reset()
        rt1.reset()
        lt1.reset()
        mem1.reset()
        rumble1.reset()
        xl1.reset()
        xr1.reset()
        xu1.reset()
        xd1.reset()
        xul1.reset()
        xur1.reset()
        xdl1.reset()
        xdr1.reset()
        kt_dr1.reset()
        kt_dl1.reset()
        kt_dd1.reset()
        kt_du1.reset()
        kt_z1.reset()
        kt_b1.reset()
        kt_a1.reset()
        kt_cr1.reset()
        kt_cl1.reset()
        kt_cd1.reset()
        kt_cu1.reset()
        kt_rt1.reset()
        kt_lt1.reset()
        kt_mem1.reset()
        kt_rumble1.reset()
        dr2.reset()
        dl2.reset()
        dd2.reset()
        du2.reset()
        start2.reset()
        z2.reset()
        b2.reset()
        a2.reset()
        cr2.reset()
        cl2.reset()
        cd2.reset()
        cu2.reset()
        rt2.reset()
        lt2.reset()
        mem2.reset()
        rumble2.reset()
        xl2.reset()
        xr2.reset()
        xu2.reset()
        xd2.reset()
	}
}