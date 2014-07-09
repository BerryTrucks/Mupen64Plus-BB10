import bb.cascades 1.0
import bb.cascades.pickers 1.0

Sheet {
    id: shortcutSheet
    
    property alias title: titleField.text
    property string location: ""
    property string defaultIcon: ""

	Page {
        titleBar: TitleBar {
            title: qsTr("Shortcut")
            scrollBehavior: TitleBarScrollBehavior.Sticky
            acceptAction: ActionItem {
                title: qsTr("Create")

                onTriggered: {
                    var res = iconField.text
                    if (res.length == 0) {
                        res = defaultIcon
                    }
                    if (_frontend.createShortcut(titleField.text, res, location, loadplay.selectedIndex == 0)) {
                    	shortcutSheet.close()
                    }
                }
            }
            dismissAction: ActionItem {
                title: qsTr("Cancel")

                onTriggered: {
                    shortcutSheet.close()
                }
            }
        }
        ScrollView {
            Container {
                leftPadding: 15.0
                topPadding: 15.0
                rightPadding: 15.0
                bottomPadding: 15.0
                Container {
                    Label {
                        text: qsTr("Title")
                    }
                }
                Container {
                    TextField {
                        id: titleField
                        hintText: qsTr("")
                        validator: Validator {
                            mode: ValidationMode.Immediate
                            errorMessage: qsTr("Invalid shortcut name (name must be less than 21 characters long and can only include letters, numbers, and spaces)")
    
                            onValidate: {
                                valid = _frontend.isValidFilename(titleField.text)
                            }
                        }
                    }
                }
                DropDown {
					id: loadplay
                    title: qsTr("Action")
                    
                    Option {
                        text: qsTr("Play")
                        selected: true
                    }
                    
                    Option {
                        text: qsTr("Load")
                    }
                }
                Container {
                    topPadding: 30.0
                    Label {
                        text: qsTr("Icon")
                    }
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    TextField {
                        id: iconField
                        hintText: qsTr("Use Boxart")
                        verticalAlignment: VerticalAlignment.Center
                        enabled: false
                    }
                    Button {
                        verticalAlignment: VerticalAlignment.Center
                        text: qsTr("...")
                        maxWidth: 20.0
                        
                        onClicked: {
                            picker.open()
                        }
                    }
                }
                Container {
                    topPadding: 15.0
                    horizontalAlignment: HorizontalAlignment.Fill
    	            ImageView {
    	                id: iconImage
    	                imageSource: defaultIcon
    	                horizontalAlignment: HorizontalAlignment.Center
                        scalingMethod: ScalingMethod.AspectFit
                    }
    	        }
            }
        }
    }
	
    attachedObjects: [
        FilePicker {
            id: picker
            
            property string selectedFile
            
            title: "Rom Selector"
            filter: ["*.png", "*.jpg", "*.jpeg", "*.jfif", "*.jif", "*.jpe"]
            type: FileType.Picture

            onFileSelected: {
                iconField.text = "file://" + selectedFiles[0]
                iconImage.imageSource = iconField.text
            }
        }
	]
    
    onOpened: {
        titleField.validator.validate()
    }
}
