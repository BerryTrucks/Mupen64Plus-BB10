import bb.cascades 1.0

Container {
    property int player  
    
    DropDown {
        id: _device
        title: "Input Device"
        
        Option {
            id: _disabled
            text: "Disabled"
            selected: _frontend.getInputValue(player, "present") == 0
        }
        Option {
            text: "Touchscreen"
            selected: !(_disabled.selected) && _frontend.getInputValue(player, "device") == -3
        }
        Option {
            text: "Keyboard"
            selected: !(_disabled.selected) && _frontend.getInputValue(player, "device") == -2
        }
        Option {
            text: "GamePad"
            selected: !(_disabled.selected) && _frontend.getInputValue(player, "device") < -3
        }
        
        onSelectedIndexChanged: {
            if(selectedIndex == 0){
               _frontend.setInputValue(player, "present", 0);
            } else {
                if (selectedIndex == 1) {
                    _frontend.setInputValue(player, "device", -3);
                } else if (selectedIndex == 2) {
                    _frontend.setInputValue(player, "device", -2);
                    _skin.selectedIndex = 0
                } else if (selectedIndex == 3) {
                    _gptype.selectedIndex = 0
                    _skin.selectedIndex = 0
                }
                _frontend.setInputValue(player, "present", 1);
            }
            
        }
    }
    
    Divider {}
	
	//Touchscreen
	Container {
	    id:_touch
	    visible: _device.selectedIndex == 1
	    
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
        visible: _device.selectedIndex == 2
		Container {
		    id:_keyboard
		    
		    leftPadding: 20
		    rightPadding: 20
		    
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
		}
    }
	
	//gamepad
	Container {
	    id:_gamepad
	    
        visible: _device.selectedIndex == 3
        DropDown {
            id: _gptype
            title: qsTr("GamePad Type")
            Option {
                text: qsTr("Generic")
                imageSource: "asset:///images/ca_bluetooth.png"
                selected: true
            }
            Option {
                text: qsTr("Moga")

                imageSource: "asset:///images/moga.png"
            }
            
            onCreationCompleted: {
                if (_frontend.getInputValue(player,"device") == -5) {
                    selectedIndex = 1
                }
                else {
                    selectedIndex = 0
                }
            }
            
            onSelectedIndexChanged: {
                if (selectedIndex == 0) {
                    _frontend.setInputValue(player, "device", -4);
                }
                else if (selectedIndex == 1) {
                    _frontend.setInputValue(player, "device", -5);
                }
            }
        }
        ScrollView {
            visible: _gptype.selectedIndex == 1
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
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
                MogaButtonMap {
                    id: xl2
                    button: "X Axis Left"
                }
                MogaButtonMap {
                    id: xr2
                    button: "X Axis Right"
                }
                MogaButtonMap {
                    id: xu2
                    button: "Y Axis Up"
                }
                MogaButtonMap {
                    id: xd2
                    button: "Y Axis Down"
                }
            }
        }
        Container {
            visible: _gptype.selectedIndex == 0
            Label {
                text: qsTr("Coming Soon")
                textStyle.color: Color.Red
                horizontalAlignment: HorizontalAlignment.Center
            }
            Divider {}
	        ListView {
	            dataModel: _frontend.devices
	            
	            listItemComponents: [
	                ListItemComponent {
	                    type: ""
	
	                    Container {
	                        Container {
	                            layout: StackLayout {
	                                orientation: LayoutOrientation.LeftToRight
	                            }
	                            Container {
	                                verticalAlignment: VerticalAlignment.Center
	                                ImageView {
	                                    imageSource: "asset:///images/ca_bluetooth.png"
	                                }
	                            }
	                            Container {
	                                leftPadding: 15.0
	                                verticalAlignment: VerticalAlignment.Center
	                                Container {
	                                    Label {
	                                        text: ListItemData.name
	                                        textStyle.fontSize: FontSize.Large
	                                    }
	                                }
	                                Container {
	                                    Label {
	                                        textStyle.fontSize: FontSize.Small
	                                        text: ListItemData.description
	                                    }
	                                }
	                            }
	                        }
	                        Divider {
	                        }
	                    }
	                }
	            ]
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