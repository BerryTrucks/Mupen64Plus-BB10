import bb.cascades 1.0

Sheet {
    id: historySheet
    
    signal loadROM(string path)
    signal playROM(string path)
    
	Page {
	    titleBar: TitleBar {
	        title: qsTr("History")
	        dismissAction: ActionItem {
	            title: qsTr("Cancel")
	            
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
                    historySheet.close()
	                playROM(location)
	            }
	            
	            function remove(uuid) {
	                _frontend.removeFromHistory(uuid)
	            }

                onTriggered: {
                    historySheet.close()
	                var selectedItem = dataModel.data(indexPath)
	                loadROM(selectedItem.location)
	            }
	        }
	    }
	}
}
