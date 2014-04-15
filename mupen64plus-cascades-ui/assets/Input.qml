import bb.cascades 1.0

Page {   
    titleBar: TitleBar {
        id: titleBar
        visibility:  ChromeVisibility.Visible
        scrollBehavior: TitleBarScrollBehavior.Sticky
        kind: TitleBarKind.FreeForm

        kindProperties: FreeFormTitleBarKindProperties {
        	content: Container {
        	    verticalAlignment: VerticalAlignment.Fill
        	    leftPadding: 15.0
        	    
        	    layout: DockLayout { }
        	    
        	    Label {
                    text: qsTr("Input Setings")
                    textStyle.color: Color.White
                    textStyle.fontSize: FontSize.Large
                    verticalAlignment: VerticalAlignment.Center
                }
        	}
        	
        	expandableArea {
                content: Container {
                    preferredWidth: 768
                    horizontalAlignment: HorizontalAlignment.Center
                    background: _frontend.colorIndex == 0 ? Color.create("#E8E8E8") : Color.Black;
                    bottomPadding: 10
                    topPadding: 10
                    
                    SegmentedControl {
                        id: _player
                        Option {
                            id: player1
                            text: "Player 1"
                            selected: true
                        }
                        Option {
                            id: player2
                            text: "Player 2"
                        }
                        Option {
                            id: player3
                            text: "Player 3"
                        }
                        Option {
                            id: player4
                            text: "Player 4"
                        }
                    }
                }
        	    
        	    toggleArea: TitleBarExpandableAreaToggleArea.IndicatorOnly
        	}
        }
    }
    
    Container {
	    
	    Container{
	        topPadding: 10
	        rightPadding: 20
	        leftPadding: 20
	        
	        
	        Container {
	            Devices {
	                id: _devices
	                player: _player.selectedIndex
	            }
	        }
	    }
	}
    
    actions: [
        ActionItem {
            title: qsTr("Set Defaults")
            imageSource: "asset:///images/default_icon.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            
            onTriggered: {
                _devices.reset()
            }
        }
    ]
}