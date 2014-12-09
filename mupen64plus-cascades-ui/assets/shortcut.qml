import bb.cascades 1.0
import bb.cascades.pickers 1.0

Sheet {
    id: shortcutSheet
    
    property alias title: titleField.text
    property string location: ""
    property string defaultIcon: ""
    property string clearIcon: ""
    
    onDefaultIconChanged: {
        if (defaultIcon.length > 0 && _frontend.fileExists(defaultIcon)) {
            iconDropdown.insert(0, boxartOption)
            iconDropdown.selectedIndex = 0
        }
    }
    
    onClearIconChanged: {
        if (clearIcon.length > 0 && _frontend.fileExists(clearIcon)) {
            if (iconDropdown.count() == 1) {
                iconDropdown.insert(0, clearOption)
                iconDropdown.selectedIndex = 0
            }
            else {
                iconDropdown.insert(1, clearOption)
            }
        }
    }

	Page {
        titleBar: TitleBar {
            title: qsTr("Shortcut")
            scrollBehavior: TitleBarScrollBehavior.Sticky
            acceptAction: ActionItem {
                title: qsTr("Create")

                onTriggered: {
                    var res = iconField.text
                    if (boxartOption.selected) {
                        res = defaultIcon
                    }
                    else if (clearOption.selected) {
                        res = clearIcon
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
                bottomPadding: 15.0

                Header {
                    title: qsTr("General")
                }
                Container {
                    leftPadding: 15.0
                    rightPadding: 15.0
                    topPadding: 15
                    TextField {
                        id: titleField
                        hintText: qsTr("Shortcut Title")
                        validator: Validator {
                            mode: ValidationMode.Immediate
                            errorMessage: qsTr("Invalid shortcut name (name must be less than 21 characters long and can only include letters, numbers, and spaces)")

                            onValidate: {
                                valid = _frontend.isValidFilename(titleField.text)
                            }
                        }
                    }
                }
                Container {
                    rightPadding: 15.0
                    leftPadding: 15.0
                    topPadding: 15.0
                    bottomPadding: 15.0
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
                }
                Header {
                    title: qsTr("Icon")
                }

                Container {
                    leftPadding: 15.0
                    rightPadding: 15.0
                    topPadding: 15.0
                    DropDown {
                        id: iconDropdown
                        selectedIndex: 0
                        title: qsTr("Image Location")
                        Option {
                            id: customOption
                            text: qsTr("Custom Image")
                        }
                        
                        onSelectedIndexChanged: {
                            if (boxartOption.selected) {
                                iconImage.imageSource = defaultIcon
                                iconField.text = ""
                            }
                            else if (clearOption.selected) {
                                iconImage.imageSource = clearIcon
                                iconField.text = ""
                            }
                            else {
                                iconImage.resetImage()
                            }
                        }
                    }
                }
                Container {
                    visible: customOption.selected
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    topPadding: 15.0
                    leftPadding: 15.0
                    rightPadding: 15.0
                    TextField {
                        id: iconField
                        hintText: qsTr("Image Location")
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
            filter: [ "*.png", "*.jpg", "*.jpeg", "*.jfif", "*.jif", "*.jpe" ]
            type: FileType.Picture

            onFileSelected: {
                iconField.text = "file://" + selectedFiles[0]
                iconImage.imageSource = iconField.text
            }
        },
        Option {
            id: boxartOption
            text: qsTr("Boxart")
        },
        Option {
            id: clearOption
            text: qsTr("Clear Image")
        }
    ]

    onOpened: {
        titleField.validator.validate()
    }
}
