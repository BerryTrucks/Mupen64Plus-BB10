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
            selected: false
        }
        
        onSelectedIndexChanged: {
            if(selectedIndex == 0){
               _frontend.setInputValue(player, "present", 0);
            } else {
                if (selectedIndex == 1) {
                    _frontend.setInputValue(player, "device", -3);
                } else if (selectedIndex == 2) {
                    _frontend.setInputValue(player, "device", -2);
                } else if (selectedIndex == 3) {
                    tmp = 1234;
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
		        button: "DPad R"
		    }
		    ButtonMap { 
		        button: "DPad L"
		    }
		    ButtonMap { 
		        button: "DPad D"
		    }
		    ButtonMap { 
		        button: "DPad U"
		    }
		    ButtonMap { 
		        button: "Start" 
		    }
		    ButtonMap { 
		        button: "Z Trig"
		    }
		    ButtonMap { 
		        button: "B Button"
		    }
		    ButtonMap { 
		        button: "A Button"
		    }
		    ButtonMap { 
		        button: "C Button R"
		    }
		    ButtonMap { 
		        button: "C Button L"
		    }
		    ButtonMap { 
		        button: "C Button D"
		    }
		    ButtonMap { 
		        button: "C Button U"
		    }
		    ButtonMap { 
		        button: "R Trig"
		    }
		    ButtonMap { 
		        button: "L Trig"
		    }
		    ButtonMap { 
		        button: "Mempak switch"
		    }
		    ButtonMap { 
		        button: "Rumblepak switch"
		    }
		    ButtonMap { 
		        button: "X Axis Left"
		    }
		    ButtonMap { 
		        button: "X Axis Right"
		    }
		    ButtonMap { 
		        button: "Y Axis Up"
		    }
		    ButtonMap { 
		        button: "Y Axis Down"
		    }
		}
    }
	
	//gamepad
	Container {
	    id:_gamepad
	    
	    visible: _device.selectedIndex == 3
	    Label {
	         text: "GamePad to be added soon"   
	    }
	}
}