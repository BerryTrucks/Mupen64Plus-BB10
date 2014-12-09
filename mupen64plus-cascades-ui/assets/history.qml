import bb.cascades 1.0

Sheet {
    id: historySheet
    peekEnabled: false
    
    property bool loading: false
    property bool useGrid: false
    
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
                visible: !useGrid

	            listItemComponents: [
	                ListItemComponent {
	                    type: ""
	                    
                        content: 
                        Container {
                            id: _itemContainer
                            layout: DockLayout {
                            }
                            Container {
                                topPadding: 10.0
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Fill
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
                            }
                            ImageView {
                                horizontalAlignment: HorizontalAlignment.Right
                                imageSource: "asset:///images/pinned.png"
                                visible: ListItemData.pinned
                            }
                            
                            contextActions: [
                                ActionSet {
                                    title: ListItemData.name
                                    subtitle: ListItemData.time
                                    
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
                                                _itemContainer.ListItem.view.createShortcut(ListItemData.name, ListItemData.location, ListItemData.resource, ListItemData.clear)
                                            }
                                        },
                                        ActionItem {
                                            title: qsTr("Rename")
                                            imageSource: "asset:///images/ic_rename.png"
                                            
                                            onTriggered: {
                                                _itemContainer.ListItem.view.changeName(ListItemData.uuid)
                                            }
                                        },
                                        ActionItem {
                                            title: ListItemData.pinned ? qsTr("Unpin") : qsTr("Pin to Top")
                                            imageSource: ListItemData.pinned ? "asset:///images/unpin.png" : "asset:///images/pin.png"
                                            
                                            onTriggered: {
                                                _itemContainer.ListItem.view.togglePinned(ListItemData.uuid, ListItemData.pinned)
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
                
                function changeName(uuid) {
                    _frontend.changeHistoryName(uuid)
                }
                
                function togglePinned(uuid, pinned) {
                    if (pinned) {
                        _frontend.unpinHistory(uuid)
                    }
                    else {
                        _frontend.pinHistory(uuid)
                    }
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
                
                function createShortcut(name, location, icon, clear) {
                    var sheet = shortcutSheet.createObject(_frontend)
                    sheet.title = name
                    sheet.location = location
                    sheet.defaultIcon = icon
                    sheet.clearIcon = clear
                    sheet.open()
                }
            }
            
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                leftPadding: 5
                rightPadding: 5
                ListView {
                    dataModel: _frontend.history
                    visible: useGrid
                    
                    layout: GridListLayout {
                        columnCount: _frontend.emuWidth > 1400 ? 4 : 2
                        horizontalCellSpacing: 10
                        verticalCellSpacing: 10
                    }
                    
                    listItemComponents: [
                        ListItemComponent {
                            type: ""
                            
                            content: 
                            Container {
                                id: _itemContainerGrid
                                layout: DockLayout {
                                }
                                property int charsize: 10
                                property int theme: 0
                                maxHeight: _itemContainerGrid.charsize - 35.0
                                
                                Container {
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Fill
                                    layout: DockLayout {
                                    }
                                    ImageView {
                                        horizontalAlignment: HorizontalAlignment.Fill
                                        verticalAlignment: VerticalAlignment.Fill
                                        imageSource: theme == 0 ? "asset:///images/borderfade.amd" : "asset:///images/borderfadew.amd"
                                        //imageSource: ListItemData.pinned ? "asset:///images/borderpinned.png" : "asset:///images/border.9.png"
                                    }
                                }
                                Container {
                                    leftPadding: 6
                                    rightPadding: 6
                                    topPadding: 6
                                    bottomPadding: 6
                                    horizontalAlignment: HorizontalAlignment.Fill
                                    verticalAlignment: VerticalAlignment.Fill
                                    layout: DockLayout {
                                    }
                                    Container {
                                        horizontalAlignment: HorizontalAlignment.Fill
                                        verticalAlignment: VerticalAlignment.Fill
                                        background: theme == 0 ? Color.White : Color.Black
                                        layout: DockLayout {
                                        }
                                        Container {
                                            horizontalAlignment: HorizontalAlignment.Fill
                                            verticalAlignment: VerticalAlignment.Fill
                                            layout: StackLayout {
                                                orientation: LayoutOrientation.TopToBottom
                                            }
                                            Container {
                                                horizontalAlignment: HorizontalAlignment.Center
                                                //verticalAlignment: VerticalAlignment.Top
                                                layoutProperties: StackLayoutProperties {
                                                    spaceQuota: 1
                                                }
                                                topPadding: 20.0
                                                layout: DockLayout {
                                                }
                                                Container {
                                                    horizontalAlignment: HorizontalAlignment.Center
                                                    verticalAlignment: VerticalAlignment.Center
                                                    ImageView {
                                                        imageSource: ListItemData.hasclear ? ListItemData.clear : ListItemData.resource
                                                        scalingMethod: ScalingMethod.AspectFit
                                                        maxWidth: _itemContainerGrid.charsize - 40.0
                                                        maxHeight: (_itemContainerGrid.charsize - 40.0) * 0.75
                                                        minHeight: 86.0
                                                        minWidth: 125.0
                                                        preferredWidth: _itemContainerGrid.charsize - 40.0
                                                    }
                                                }
                                            }
                                            Container {
                                                horizontalAlignment: HorizontalAlignment.Center
                                                //verticalAlignment: VerticalAlignment.Bottom
                                                bottomPadding: 20.0
                                                Label {
                                                    text: ListItemData.name
                                                    textStyle.fontSize: FontSize.Large
                                                    maxWidth: _itemContainerGrid.charsize - 20.0
                                                }
                                            }
                                        }
                                        Container {
                                            horizontalAlignment: HorizontalAlignment.Right
                                            verticalAlignment: VerticalAlignment.Top
                                            visible: ListItemData.pinned
                                            ImageView {
                                                imageSource: "asset:///images/pinned2.png"
                                            }
                                        }
                                    }
                                }
                                
                                contextActions: [
                                    ActionSet {
                                        title: ListItemData.name
                                        subtitle: ListItemData.time
                                        
                                        actions: [
                                            ActionItem {
                                                title: qsTr("Play")
                                                imageSource: "asset:///images/ic_open.png"
                                                
                                                onTriggered: {
                                                    _itemContainerGrid.ListItem.view.play(ListItemData.location)
                                                }
                                            },
                                            ActionItem {
                                                title: qsTr("Create Shortcut")
                                                imageSource: "asset:///images/ic_add_home.png"
                                                
                                                onTriggered: {
                                                    _itemContainerGrid.ListItem.view.createShortcut(ListItemData.name, ListItemData.location, ListItemData.resource, ListItemData.clear)
                                                }
                                            },
                                            ActionItem {
                                                title: qsTr("Rename")
                                                imageSource: "asset:///images/ic_rename.png"
                                                
                                                onTriggered: {
                                                    _itemContainerGrid.ListItem.view.changeName(ListItemData.uuid)
                                                }
                                            },
                                            ActionItem {
                                                title: ListItemData.pinned ? qsTr("Unpin") : qsTr("Pin to Top")
                                                imageSource: ListItemData.pinned ? "asset:///images/unpin.png" : "asset:///images/pin.png"
                                                
                                                onTriggered: {
                                                    _itemContainerGrid.ListItem.view.togglePinned(ListItemData.uuid, ListItemData.pinned)
                                                }
                                            },
                                            DeleteActionItem {
                                                onTriggered: {
                                                    _itemContainerGrid.ListItem.view.remove(ListItemData.uuid)
                                                }
                                            }
                                        ]
                                    }
                                ]
                                
                                onCreationCompleted: {
                                    charsize = _itemContainerGrid.ListItem.view.charSize()
                                    theme = _itemContainerGrid.ListItem.view.theme()
                                }
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
                    
                    function changeName(uuid) {
                        _frontend.changeHistoryName(uuid)
                    }
                    
                    function togglePinned(uuid, pinned) {
                        if (pinned) {
                            _frontend.unpinHistory(uuid)
                        }
                        else {
                            _frontend.pinHistory(uuid)
                        }
                    }
                    
                    function charSize() {
                        return (_frontend.emuHeight - 20) / 2.0
                    }
                    
                    function theme() {
                        return _settings.Settings.Theme
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
                    
                    function createShortcut(name, location, icon, clear) {
                        var sheet = shortcutSheet.createObject(_frontend)
                        sheet.title = name
                        sheet.location = location
                        sheet.defaultIcon = icon
                        sheet.clearIcon = clear
                        sheet.open()
                    }
                    
                    onCreationCompleted: {
                        _frontend.playHistoryROM.connect(playHistoryROM)
                        _frontend.loadHistoryROM.connect(loadHistoryROM)
                        layout.cellAspectRatio = charSize() / (charSize() - 35.0)
                    }
                }
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
	    
	    actions: [
	        ActionItem {
	            ActionBar.placement: ActionBarPlacement.OnBar
                function resetAction() {
                    if (_settings.Settings.UseGridInHistory) {
                        imageSource = "asset:///images/ic_view_list.png"
                        title = qsTr("View List")
                        useGrid = true
                    }
                    else {
                        imageSource = "asset:///images/ic_view_grid.png"
                        title = qsTr("View Grid")
                        useGrid = false
                    }
	            }
                
                onTriggered: {
                    _settings.Settings.UseGridInHistory = !_settings.Settings.UseGridInHistory
                    resetAction()
                }
	            
	            onCreationCompleted: {
	                resetAction()
	            }
	        }
	    ]
    }
    
    attachedObjects: [
        ComponentDefinition {
            id: shortcutSheet
            source: "shortcut.qml"
        }
    ]
}
