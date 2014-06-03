import bb.cascades 1.0

Sheet {
    id: historySheet
    
    property bool loading: false
    
    signal loadROM(string path)
    signal playROM(string path)
    
	Page {
	    titleBar: TitleBar {
	        title: qsTr("History")
	        dismissAction: ActionItem {
	            title: qsTr("Cancel")
	            enabled: !loading

	            onTriggered: {
	                historySheet.close()
	            }
	        }
	    }
	    Container {
	        layout: DockLayout { }

            ListView {
	            dataModel: _frontend.history

	            listItemComponents: [
	                ListItemComponent {
	                    type: ""
	                    
                        content: 
                        Container {
                            id: _itemContainer
                            topPadding: 10.0
                            Container {
                                layout: StackLayout {
                                    orientation: LayoutOrientation.LeftToRight
                                }
                                horizontalAlignment: HorizontalAlignment.Left
                                leftPadding: 10.0
                                Container {
                                    verticalAlignment: VerticalAlignment.Center
                                    Container {
                                        topPadding: 5.0
                                        ImageView {
                                            imageSource: ListItemData.resource
                                            scalingMethod: ScalingMethod.AspectFit
                                            maxWidth: 256.0
                                            maxHeight: 86.0
                                            minHeight: 86.0
                                            minWidth: 125.0
                                            preferredHeight: 86.0
                                            preferredWidth: 125.0
                                        }
                                    }
                                }
                                Container {
                                    verticalAlignment: VerticalAlignment.Center
                                    leftPadding: 5.0
                                    Container {
                                        Label {
                                            text: ListItemData.name
                                            textStyle.fontSize: FontSize.Large
                                        }
                                    }
                                    Container {
                                        Label {
                                            text: ListItemData.time
                                            textStyle.fontSize: FontSize.Small
                                        }
                                    }
                                }
                            }
                            Divider {
                            }
                            
                            contextActions: [
                                ActionSet {
                                    title: ListItemData.name
                                    
                                    actions: [
                                        ActionItem {
                                            title: qsTr("Play")
                                            imageSource: "asset:///images/ic_open.png"
                                            
                                            onTriggered: {
                                                _itemContainer.ListItem.view.play(ListItemData.location)
                                            }
                                        },
                                        ActionItem {
                                            id: shortcutAction
                                            title: qsTr("Create Shortcut")
                                            imageSource: "asset:///images/ic_add_home.png"
                                            
                                            onTriggered: {
                                                _itemContainer.ListItem.view.createShortcut(ListItemData.name, ListItemData.location, ListItemData.resource)
                                            }
                                        },
                                        DeleteActionItem {
                                            onTriggered: {
                                                _itemContainer.ListItem.view.remove(ListItemData.uuid)
                                            }
                                        }
                                    ]
                                }
                            ]
                        }
	                }
	            ]

                function play(location) {
                    loading = true
                    _frontend.invokePlayHistoryROM(location)
	            }
	            
	            function remove(uuid) {
	                _frontend.removeFromHistory(uuid)
	            }

                onTriggered: {
                    loading = true
                    var selectedItem = dataModel.data(indexPath)
                    _frontend.invokeLoadHistoryROM(selectedItem.location)
	            }
                
                function playHistoryROM(rom) {
                    playROM(rom)
                    historySheet.close()
                }
                
                function loadHistoryROM(rom) {
                    loadROM(rom)
                    historySheet.close()
                }
                
                function createShortcut(name, location, icon) {
                    var sheet = shortcutSheet.createObject()
                    sheet.title = name
                    sheet.location = location
                    sheet.defaultIcon = icon
                    sheet.open()
                }
                
                onCreationCompleted: {
                    _frontend.playHistoryROM.connect(playHistoryROM)
                    _frontend.loadHistoryROM.connect(loadHistoryROM)
                }
                
                attachedObjects: [
                    ComponentDefinition {
                        id: shortcutSheet
                        source: "shortcut.qml"
                    }
                ]
	        }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                background: Color.create("#74000000")
                visible: loading

                layout: DockLayout {
                }
                ActivityIndicator {
                    running: loading
                    minWidth: 200.0
                    minHeight: 200.0
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Center
                }
            }
	    }
	}
}
