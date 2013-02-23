import bb.cascades 1.0

Container {
    property alias button: _button.text
    
    bottomPadding: 20
    
    Container {
        
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
	        minWidth: 200
	        text: _frontend.getInputValue(player, button)
	        
	        //onTextChanged: {
	        //    _frontend.setInputValue(player, button, parseInt(text))
	        //}
	        onClicked: {
	            var mapButton = _frontend.mapButton();
	            if(mapButton != -1){
	                _frontend.setInputValue(player, button, mapButton);
	                _mapping.text = mapButton.toString();
	            }
	        }
	    }
	}
    
    Divider {}
    
}