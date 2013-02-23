import bb.cascades 1.0
import bb.cascades.pickers 1.0

Page {
    id: menu
    property int videoPlugin
    property bool sound
    
    actionBarVisibility: {
        if(emulatorVisable){
            ChromeVisibility.Hidden
        } else {
            ChromeVisibility.Visible
        }
    }
    actions: [
        ActionItem {
            title: "Play"
            ActionBar.placement: ActionBarPlacement.OnBar
            imageSource: "asset:///images/ic_open.png"
            onTriggered: {
                _frontend.video = videoPlugin;
                _frontend.audio = sound;
                if(_frontend.rom == ""){
                    //Do something to focus on select rom box
                } else {
                    OrientationSupport.supportedDisplayOrientation = 
                            SupportedDisplayOrientation.DisplayLandscape;
                    emulatorVisable = true;
                }
            }
        }
    ]
    
    property alias romDirectory: picker.directories
    property bool boxartEnabled
    property bool romLoading: false
    property bool useNetImage: false
    
    titleBar: TitleBar {
        id: titleBar
        title: "Mupen64Plus-BB"
        visibility:  {
            if(emulatorVisable) {
                ChromeVisibility.Hidden
            } else {
                ChromeVisibility.Visible
            }
        }
    }
    
    // Content Container
    Container {
        horizontalAlignment: HorizontalAlignment.Center
        
        Container {
            layout: AbsoluteLayout {}
            
            ForeignWindowControl {
	            id: _myForeignWindow
	            objectName: "myForeignWindow"
	            keyInputForwardingEnabled: true
	            windowId: "emulator_m64p"
	
	            visible: emulatorVisable
	            layoutProperties: AbsoluteLayoutProperties {
	                positionX: 0
	                positionY: 0
	            }
	            preferredWidth: 1280
	            preferredHeight: 768
	            
	            updatedProperties: WindowProperty.Size | WindowProperty.Position | WindowProperty.Visible
	            
	            onBoundToWindowChanged: {
	                if(boundToWindow){
	                    emulatorVisable = true;
	                } else {
	                    emulatorVisable = false;
	                }
	            }
	        }
        }
        
	    Container {
	        topPadding: 10
	        visible: !emulatorVisable
	        //background: backgroundPaint.imagePaint
	
	        //attachedObjects: [
	        //    ImagePaintDefinition {
	        //        id: backgroundPaint
	        //       imageSource: "asset:///images/Background.png"
	        //   }
	        //]
	        Container {
	            minHeight: 250
	            horizontalAlignment: HorizontalAlignment.Center
	            
	            ImageView {
				    id: myImageView
				    imageSource: "asset:///images/mupen64plus.png"
				}
	        }  
	
	        // Top Container with a RadioButtonGroup and title
	        Container {
	            preferredWidth: 650
	            horizontalAlignment: HorizontalAlignment.Center
	            Container {
	                preferredWidth: 650
	                bottomPadding: 50
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
	                        picker.open();
	                        romLoading = true;
	                    }
	                }
	            }
	            
	        } // Top Container
	        
	        Divider {}
	        
	        //Boxart
	        Container {
                preferredHeight: 500
                preferredWidth: 768
                topPadding: 50
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
	            filter: ["*.n64", "*.z64", "*.v64"]
	            type: FileType.Other
	
	            onFileSelected: {
	                _frontend.rom = selectedFiles[0];
	                selectedFile = _frontend.rom.substr(_frontend.rom.lastIndexOf('/')+1);
	                
	                if(boxartEnabled){
	                    var tmp = picker.selectedFile.indexOf("(")
		                if(tmp == -1){
		                    _tracker.imageSource = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, picker.selectedFile.lastIndexOf(".")).trim() + ".jpg";
		                } else {
		                    _tracker.imageSource = "file:///accounts/1000/shared/misc/n64/.boxart/" + picker.selectedFile.substr(0, tmp).trim() + ".jpg";
		                }
	                }
	                _frontend.LoadRom();
	                _frontend.createCheatsPage();  
	            }
	        },
	        OrientationHandler {
	              onOrientationChanged: {
	                  if(OrientationSupport.supportedDisplayOrientation == SupportedDisplayOrientation.DisplayLandscape){
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
	            }
	        }
	    ]           
	} 
} // Page