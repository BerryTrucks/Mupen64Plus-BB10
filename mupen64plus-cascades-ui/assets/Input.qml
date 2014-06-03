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
                    textStyle.fontSize: FontSize.Large
                    verticalAlignment: VerticalAlignment.Center
                    
                    onCreationCompleted: {
                        if (!_frontend.isOSThree && _frontend.themeIndex == 0) {
                            textStyle.color = Color.White
                        }
                    }
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
	            layout: DockLayout { }
	            Devices {
	                id: _devices1
                    visible: _player.selectedIndex == 0
                    player: 0
                }
                Devices {
                    id: _devices2
                    visible: _player.selectedIndex == 1
                    player: 1
                }
                Devices {
                    id: _devices3
                    visible: _player.selectedIndex == 2
                    player: 2
                }
                Devices {
                    id: _devices4
                    visible: _player.selectedIndex == 3
                    player: 3
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