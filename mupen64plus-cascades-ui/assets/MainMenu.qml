import bb.cascades 1.0
import bb.cascades.pickers 1.0

Page {
    id: menu
    property int videoPlugin
    property bool sound
    property alias romDirectory: picker.directories
    property bool boxartEnabled
    property bool romLoading: false
    property bool useNetImage: false
    property bool startNow: false
    property bool running: false
    property variant emuSheet

    Menu.definition: MenuDefinition {
        id: appMenu
        actions: [
            ActionItem {
                title: "Save State"
                imageSource: "asset:///images/save_load.png"
                onTriggered: {
                    _frontend.SaveState();
                }
            },
            ActionItem {
                title: "Load State"
                imageSource: "asset:///images/save_load.png"
                onTriggered: {
                    _frontend.LoadState();
                }
            },
            ActionItem {
                title: "Toggle Overlay"
                imageSource: "asset:///images/overlay.png"
                onTriggered: {
                    _frontend.LoadTouchOverlay();
                }
            },
            ActionItem {
                title: "Menu (Broken)"
                imageSource: "asset:///images/home.png"
                //enabled: false //TODO: Issues with restarting emu, SDL audio plugin crashes, can fix by not PluginUnload audio, but emu still crashes randomly, and on 3rd open.
                onTriggered: {
                    _frontend.ExitEmulator();
                    OrientationSupport.supportedDisplayOrientation = 
                    SupportedDisplayOrientation.DisplayPortrait;
                    running = false
                } 
            },
            ActionItem {
                title: "Close"
                imageSource: "asset:///images/ic_cancel.png"
            }
        ]
    }
    
    function startEmulator() {
        _frontend.video = videoPlugin;
        _frontend.audio = sound;
        if(_frontend.rom == ""){
            //Do something to focus on select rom box
        } else {
            emuSheet = playSheet.createObject()
            emuSheet.open()
            Application.swipeDown.connect(onSwipeDown)
            running = true
            if (_frontend.Keyboard) {
                _frontend.startEmulator(true);
            }
            else {
                OrientationSupport.supportedDisplayOrientation = 
                SupportedDisplayOrientation.DisplayLandscape;
            }
        }
    }
    
    function emulatorClosing() {
        running = false
        emuSheet.destroyLater()
    }
    
    function onSwipeDown() {
        _frontend.swipedown()
    }

    actions: [
        ActionItem {
            id: _play
            title: "Play"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_open.png"
            onTriggered: {
                startEmulator()
            }
        }
    ]
    
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
    
    function invoked(path) {
        if (!running) {
            loadROM(path)
        }
    }
    
    onCreationCompleted: {
        _frontend.hdmiDetected.connect(hdmiDetected)
        _frontend.hasHistoryChanged.connect(hasHistory)
        _frontend.invoked.connect(invoked)
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
                    verticalAlignment: VerticalAlignment.Center
                    leftPadding: 15.0
                    
                    Label {
                        text: qsTr("Mupen64Plus-BB")
                        textStyle.fontSize: FontSize.Large
                        textStyle.color: Color.White
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
        horizontalAlignment: HorizontalAlignment.Center
        
	    Container {
	        topPadding: 10

	        // Top Container with a RadioButtonGroup and title
	        Container {
	            preferredWidth: 650
	            topPadding: 50.0
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
	                    onClicked: {
	                        if (_frontend.hasStartDirectory)
	                        	picker.directories = [ _frontend.startDirectory ]
	                        picker.open();
	                        romLoading = true;
	                    }
	                }
	            }
	            
	        } // Top Container
	        
	        //Boxart
	        Container {
                preferredHeight: 500
                preferredWidth: 768
                topPadding: _frontend.Keyboard ? 5.0 : 50
                visible: boxartEnabled

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
                    image: {
                        if(_frontend.boxart && useNetImage && _frontend.boxartLoaded) 
                            _frontend.boxart.image
                        else 
                            _tracker.image;
                    }
                    visible: !romLoading
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
	
	            onFileSelected: {
	                _frontend.rom = selectedFiles[0];
	                selectedFile = _frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1);
	                _frontend.startDirectory = _frontend.rom.substr(0, _frontend.rom.length - selectedFile.length)
	                
	                if(boxartEnabled){
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
	                _frontend.LoadRom();
	                _frontend.createCheatsPage();
	            }
	        },
	        OrientationHandler {
	              onOrientationChanged: {
                      if(OrientationSupport.supportedDisplayOrientation == SupportedDisplayOrientation.DisplayLandscape) {
	                      _frontend.startEmulator(true);
	                  }
	              }
	        }, 
	        ImageTracker {
	            id: _tracker
	            //imageSource: "file://" + _frontend.rom + ".png"
	        
	            onStateChanged: {
	                /*
	                if (state == 0x2) //Loaded state
	                {
	                    myImageView.image = _tracker.image;
	                    console.log("Image Loaded: " + _tracker.imageSource);
	                } else if (state > 0x2 ) { //Error states
	                    myImageView.imageSource = "asset:///images/mupen64plus.png";
	                    console.log("Image Failed: " + _tracker.imageSource);
	                }*/
	                if (state == 0x2) {//Loaded
	                    //_frontend.boxart.image = _tracker.image;
	                    useNetImage = false;
	                    romLoading = false;
	                } else if (state == 0x3) {//Not found
	                    //console.log("Image Not Found: " + _tracker.imageSource);
	                    _frontend.loadBoxArt(picker.selectedFile);
	                    useNetImage = true;
	                    romLoading = false;
	                } else if (state > 0x3 ) { //Error states
	                    //myImageView.imageSource = "asset:///images/ps1_icon.png";
	                    //console.log("Image Failed: " + _tracker.imageSource);
	                }
	                if (startNow) {
                        startNow = false
                        startEmulator()
	                }
	            }
	        }
	    ]
	}
    
    function loadROM(path) {
        romLoading = true
        _frontend.rom = path
        picker.selectedFile = _frontend.rom.substr(_frontend.rom.lastIndexOf('/') + 1)
        
        if(boxartEnabled){
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
    
    attachedObjects: [
        ActionItem {
            id: _playon
            title: "Play On"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_hdmi.png"
            onTriggered: {
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
        ComponentDefinition {
            id: historySheet
            source: "history.qml"
        },
        ComponentDefinition {
            id: playSheet
            source: "play.qml"
        }
    ]
} // Page