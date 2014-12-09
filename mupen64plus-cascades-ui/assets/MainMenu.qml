import bb.cascades 1.0
import bb.cascades.pickers 1.0

Page {
    id: menu
    property bool romLoading: false
    property bool useNetImage: false
    property bool startNow: false
    property bool running: false
    property variant emuSheet: null
    
    function startEmulator() {
        if (_frontend.rom.length > 0) {
            emuSheet = playSheet.createObject(_frontend)
            emuSheet.open()
            Application.swipeDown.connect(onSwipeDown)
            running = true
            if (_frontend.Keyboard) {
                _frontend.startEmulator(true);
            }
            else {
                OrientationSupport.supportedDisplayOrientation = SupportedDisplayOrientation.DisplayLandscape
            }
        }
        else {
            romSelectButton.requestFocus()
        }
    }
    
    function emulatorClosing() {
        running = false
        emuSheet.destroyLater()
    }
    
    function onSwipeDown() {
        _frontend.swipedown()
    }
    
    function hdmiDetected(attached) {
        if (attached) {
            menu.addAction(_playon, ActionBarPlacement.OnBar)
        }
        else {
            menu.removeAction(_playon)
        }
    }
    
    function hasHistory() {
        if (_frontend.hasHistory) {
            menu.addAction(_history, ActionBarPlacement.OnBar)
        }
        else {
            menu.removeAction(_history)
        }
    }
    
    function invoked(path, runnow) {
        if (!running) {
            if (runnow) {
                playROM(path)
            }
            else {
            	loadROM(path)
            }
        }
    }

    function loadROM(path) {
        console.log("Load ROM from function")
        romLoading = true
        _frontend.rom = path
        picker.selectedFile = _frontend.rom.substr(_frontend.rom.lastIndexOf('/') + 1)
        
        if(_settings.Settings.BoxartScraping) {
            var tmp = picker.selectedFile.indexOf("(")
            if(tmp == -1){
                _tracker.imageSource = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, picker.selectedFile.lastIndexOf(".")).trim() + ".jpg";
            } else {
                _tracker.imageSource = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, tmp).trim() + ".jpg";
            }
        }
        else {
            _tracker.imageSource = "asset:///images/mupen64plus.png"
        }
        _frontend.LoadRom()
        _frontend.createCheatsPage()
    }
    
    function playROM(path) {
        startNow = true
        loadROM(path)
    }
    
    function onROMLoaded() {
        if (!_shortcut.loaded) {
            _shortcut.loaded = true
            menu.addAction(_shortcut, ActionBarPlacement.InOverflow)
            menu.addAction(_rominfo, ActionBarPlacement.InOverflow)
        }
    }
    
    function gameHasId(has) {
        if (has && !_gameInfo.isInList) {
            addAction(_gameInfo)
        }
        else if (!has && _gameInfo.isInList) {
            removeAction(_gameInfo)
        }
    }

    actions: [
        ActionItem {
            id: _play
            title: "Play"
            ActionBar.placement: _frontend.playPlacement
            imageSource: "asset:///images/ic_open.png"
            onTriggered: {
                startEmulator()
            }
        }
    ]
    
    onCreationCompleted: {
        _frontend.hdmiDetected.connect(hdmiDetected)
        _frontend.hasHistoryChanged.connect(hasHistory)
        _frontend.invoked.connect(invoked)
        _frontend.ROMLoaded.connect(onROMLoaded)
        _frontend.gameHasId.connect(gameHasId)
        hasHistory()
    }
    
    titleBar: TitleBar {
        id: titleBar
        kind: TitleBarKind.FreeForm
        scrollBehavior: TitleBarScrollBehavior.Sticky
        
        kindProperties: FreeFormTitleBarKindProperties {
            content: Container {
                layout: DockLayout {}
                
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    
                    ImageView {
                        imageSource: _frontend.debugMode ? (_settings.Settings.Theme == 0 ? "asset:///images/titlebar.png" : "asset:///images/titlebar_dark.png") : "asset:///images/None.png"
                    }
                }
                
                Container {
                    verticalAlignment: VerticalAlignment.Center
                    leftPadding: 15.0
                    
                    Label {
                        text: _frontend.debugMode ? qsTr("Mupen64Plus  Debug") : qsTr("Mupen64Plus ")
                        textStyle.fontSize: FontSize.Large
                        textStyle.fontWeight: FontWeight.W500
                        
                        onCreationCompleted: {
                            if (!_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                textStyle.color = Color.White
                            }
                        }
                    }
                }
                Container {
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    
                    rightPadding: 15.0
                    ImageView {
                        imageSource: "asset:///images/mupen64plus.png"
                        scalingMethod: ScalingMethod.AspectFit
                        maxWidth: 81.0
                        maxHeight: 81.0
                    }
                }
            }
        }
    }
    
    // Content Container
    Container {
        horizontalAlignment: HorizontalAlignment.Fill
        layout: DockLayout {
        }
        
	    Container {
	        topPadding: 10
	        horizontalAlignment: HorizontalAlignment.Fill

	        // Top Container with a RadioButtonGroup and title
	        Container {
	            preferredWidth: 650
	            topPadding: _frontend.Keyboard ? 10.0 : 50.0
	            horizontalAlignment: HorizontalAlignment.Center
	            Container {
	                preferredWidth: 650
                    bottomPadding: _frontend.Keyboard ? 15.0 : 50
	                horizontalAlignment: HorizontalAlignment.Center

	                layout: StackLayout {
	                    orientation: LayoutOrientation.LeftToRight
	                }

	                TextField {
	                    id: romName
	                    objectName: "romName"
	                    verticalAlignment: VerticalAlignment.Center
	                    enabled: false
	                    hintText: "Select a ROM"
	                    text: picker.selectedFile
	                }

	                Button {
	                    text: "..."
	                    horizontalAlignment: HorizontalAlignment.Right
	                    preferredWidth: 1
	                    id: romSelectButton
	                    onClicked: {
	                        if (_frontend.hasStartDirectory)
	                        	picker.directories = [ _frontend.startDirectory ]
	                        picker.open();
	                    }
	                }
	            }
	            
	        } // Top Container
	        
	        //Boxart
	        Container {
                preferredHeight: 500
                preferredWidth: 768
                topPadding: _frontend.Keyboard ? 5.0 : 50
                visible: _settings.Settings.BoxartScraping

                layout: DockLayout {}

                // The ActivityIndicator that is only active and visible while the image is loading
                ActivityIndicator {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    preferredHeight: 300

                    visible: if((_frontend.boxart && _frontend.boxart.loading) || romLoading){ true } else { false }
                    running: if((_frontend.boxart && _frontend.boxart.loading) || romLoading){ true } else { false }
                }

                // The ImageView that shows the loaded image after loading has finished without error
                ImageView {
                    id: boxart
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    scalingMethod: ScalingMethod.AspectFit
                    
                    function boxartLoaded(loaded) {
                        if (loaded) {
                            image = _frontend.boxart.image
                            visible = true
                        }
                    }
                    
                    onCreationCompleted: {
                        _frontend.boxartLoadedChanged.connect(boxartLoaded)
                        if (_settings.Settings.Theme == 0) {
                            imageSource = "asset:///images/back_white.png"
                        }
                        else {
                            imageSource = "asset:///images/back_black.png"
                        }
                    }
                }

                // The Label that shows a possible error message after loading has finished
                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    preferredWidth: 500

                    visible: if(_frontend.boxart) !_frontend.boxart.loading && !_frontend.boxart.label == ""
                    text: if(_frontend.boxart) _frontend.boxart.label
                    multiline: true
                }
            }
	        
	        onCreationCompleted: {
	            OrientationSupport.supportedDisplayOrientation = 
	                SupportedDisplayOrientation.CurrentLocked;
	        }
	        
	        attachedObjects: [
	            TextStyleDefinition {
	                id: textStyleBoldTitle
	                base: SystemDefaults.TextStyles.TitleText
	                fontWeight: FontWeight.Bold
	            }
	        ]
	    } // Content Container
	    
	    attachedObjects: [
	        FilePicker {
	            id: picker
	
	            property string selectedFile
	
	            title: "Rom Selector"
	            filter: ["*.n64", "*.z64", "*.v64", "*.zip"]
	            type: FileType.Other
	            
                directories: {
                    if(_settings.Settings.DefaultToSDCard) {
                        ["/accounts/1000/removable/sdcard"]
                    }
                    else {
                        ["/accounts/1000/shared/misc/n64/roms"]
                    }
                }
	
                onFileSelected: {
                    console.log("Loading ROM from file dialog")
                    romLoading = true
	                _frontend.rom = selectedFiles[0];
	                selectedFile = _frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1);
	                _frontend.startDirectory = _frontend.rom.substr(0, _frontend.rom.length - selectedFile.length)
	                
                    if(_settings.Settings.BoxartScraping) {
	                    var tmp = picker.selectedFile.indexOf("(")
		                if(tmp == -1){
		                    _tracker.imageSource = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, picker.selectedFile.lastIndexOf(".")).trim() + ".jpg";
		                } else {
		                    _tracker.imageSource = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, tmp).trim() + ".jpg";
		                }
	                }
                    else {
                        if (_settings.Settings.Theme == 0) {
                            _tracker.imageSource = "asset:///images/back_white.png"
                        }
                        else {
                            _tracker.imageSource = "asset:///images/back_black.png"
                        }
	                }
	                _frontend.LoadRom();
	                _frontend.createCheatsPage();
	            }
	        },
	        OrientationHandler {
	              onOrientationChanged: {
                      if (emuSheet != null && OrientationSupport.supportedDisplayOrientation == SupportedDisplayOrientation.DisplayLandscape) {
	                      _frontend.startEmulator(true);
	                  }
	              }
	        },
	        ImageTracker {
	            id: _tracker

	            onStateChanged: {
                    if (state == 0x2) {//Loaded
	                    boxart.imageSource = _tracker.imageSource
	                    boxart.visible = true
	                    useNetImage = false;
	                    romLoading = false;
	                } else if (state == 0x3) {//Not found
	                    _frontend.loadBoxArt(picker.selectedFile);
	                    useNetImage = true;
	                    romLoading = false;
	                } else if (state > 0x3 ) { //Error states
	                }
	                if (startNow) {
                        startNow = false
                        startEmulator()
	                }
	            }
	        }
	    ]
	}
    
    attachedObjects: [
        ActionItem {
            id: _playon
            title: "Play On"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_hdmi.png"
            onTriggered: {
                _frontend.useHdmi = true
                startEmulator()
            }
        },
        ActionItem {
            id: _history
            title: qsTr("History")
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_history.png"
            onTriggered: {
                var sheet = historySheet.createObject(_frontend)
                sheet.loadROM.connect(loadROM)
                sheet.playROM.connect(playROM)
                sheet.open()
            }
        },
        ActionItem {
            id: _shortcut
            title: qsTr("Create Shortcut")
            ActionBar.placement: ActionBarPlacement.InOverflow
            property bool loaded: false
            imageSource: "asset:///images/ic_add_home.png"
            
            onTriggered: {
                var sheet = shortcutSheet.createObject(_frontend)
                sheet.title = _frontend.rom.substr(_frontend.rom.lastIndexOf('/') + 1)
                sheet.location = _frontend.rom
                if(_settings.Settings.BoxartScraping) {
                    var tmp = picker.selectedFile.indexOf("(")
                    if(tmp == -1) {
                        sheet.defaultIcon = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, picker.selectedFile.lastIndexOf(".")).trim() + ".jpg";
                        sheet.clearIcon = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, picker.selectedFile.lastIndexOf(".")).trim() + "_clear.png";
                    }
                    else {
                        sheet.defaultIcon = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, tmp).trim() + ".jpg";
                        sheet.clearIcon = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, tmp).trim() + "_clear.png";
                    }
                }
                else {
                    sheet.defaultIcon = "file:///app/native/assets/images/mupen64plus.png"
                }
                sheet.open()
            }
        },
        ActionItem {
            id: _rominfo
            title: qsTr("View ROM Info")
            imageSource: "asset:///images/ic_info.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            
            onTriggered: {
                var sheet = infoSheet.createObject(_frontend)
                sheet.open()
            }
        },
        ActionItem {
            id: _gameInfo
            title: qsTr("Game Info")
            ActionBar.placement: ActionBarPlacement.InOverflow
            imageSource: "asset:///images/console.png"
            
            property bool isInList: false
            
            onTriggered: {
                _frontend.getGameInfo()
            }
        },
        ComponentDefinition {
            id: historySheet
            source: "history.qml"
        },
        ComponentDefinition {
            id: playSheet
            source: "play.qml"
        },
        ComponentDefinition {
            id: shortcutSheet
            source: "shortcut.qml"
        },
        ComponentDefinition {
            id: infoSheet
            source: "rominfo.qml"
        }
    ]
} // Page