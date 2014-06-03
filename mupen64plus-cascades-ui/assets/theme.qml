import bb.cascades 1.0

Sheet {
    id: themeSheet
    peekEnabled: false

    Page {
        titleBar: TitleBar {
            title: qsTr("Theme Settings")
            scrollBehavior: TitleBarScrollBehavior.Sticky
            dismissAction: ActionItem {
                title: qsTr("Close")

                onTriggered: {
                    themeSheet.close()
                }
            }
        }
        ScrollView {
            Container {
                Container {
                    visible: !_frontend.isOSThreeCompiled
                    Label {
                        text: qsTr("Restart the Translator app for the colour theme change to take effect.")
                        multiline: true
                    }
                    Divider {
                    }
                }
                Container {
                    topPadding: 15.0
                    DropDown {
                        title: qsTr("Visual Theme")
                        horizontalAlignment: HorizontalAlignment.Center
                        selectedIndex: _frontend.themeIndex
                        Option {
                            description: qsTr("Bright")
                        }
                        Option {
                            description: qsTr("Dark")
                        }
                        
                        onSelectedIndexChanged: {
                            _frontend.themeIndex = selectedIndex
                        }
                    }
                }
                Container {
                    topPadding: 15.0
                    DropDown {
                        id: _primary
                        horizontalAlignment: HorizontalAlignment.Center
                        selectedIndex: _frontend.primaryColourIndex
                        Option {
                            text: qsTr("Default")
                        }
                        Option {
                            description: qsTr("Custom")
                        }
                        
                        onSelectedIndexChanged: {
                            _frontend.primaryColourIndex = selectedIndex
                            if (selectedIndex == 0) {
                                primaryRed.value = 255
                                primaryGreen.value = 0
                                primaryBlue.value = 0
                            }
                        }
                        title: qsTr("Primary Colour")
                    }
                    Container {
                        rightPadding: 15.0
                        leftPadding: 15.0
                        visible: _primary.selectedIndex == 1
                        
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        Container {
                            minWidth: 100
                            minHeight: 215
                            background: Color.create(primaryRed.value / 255.0, primaryGreen.value / 255.0, primaryBlue.value / 255.0)
                        }
                        Container {
                            leftPadding: 15.0
                            Slider {
                                id: primaryRed
                                fromValue: 0
                                toValue: 255
                                value: _frontend.primaryColourRed
                                onValueChanged: {
                                    _frontend.primaryColourRed = value
                                }
                            }
                            Slider {
                                id: primaryGreen
                                fromValue: 0
                                toValue: 255
                                value: _frontend.primaryColourGreen
                                onValueChanged: {
                                    _frontend.primaryColourGreen = value
                                }
                            }
                            Slider {
                                id: primaryBlue
                                fromValue: 0
                                toValue: 255
                                value: _frontend.primaryColourBlue
                                onValueChanged: {
                                    _frontend.primaryColourBlue = value
                                }
                            }
                        }
                    }
                }
                Container {
                    topPadding: 15.0
                    visible: _primary.selectedIndex == 1
                    DropDown {
                        id: _base
                        horizontalAlignment: HorizontalAlignment.Center
                        selectedIndex: _frontend.baseColourIndex
                        Option {
                            text: qsTr("Default")
                        }
                        Option {
                            description: qsTr("Custom")
                        }
                        
                        onSelectedIndexChanged: {
                            _frontend.baseColourIndex = selectedIndex
                            if (selectedIndex == 0) {
                                baseRed.value = 0
                                baseGreen.value = 0
                                baseBlue.value = 255
                            }
                        }
                        title: qsTr("Primary Base Colour")
                    }
                    Container {
                        rightPadding: 15.0
                        leftPadding: 15.0
                        visible: _base.selectedIndex == 1
                        
                        layout: StackLayout {
                            orientation: LayoutOrientation.LeftToRight
                        }
                        Container {
                            minWidth: 100
                            minHeight: 215
                            background: Color.create(baseRed.value / 255.0, baseGreen.value / 255.0, baseBlue.value / 255.0)
                        }
                        Container {
                            leftPadding: 15.0
                            Slider {
                                id: baseRed
                                fromValue: 0
                                toValue: 255
                                value: _frontend.baseColourRed
                                onValueChanged: {
                                    _frontend.baseColourRed = value
                                }
                            }
                            Slider {
                                id: baseGreen
                                fromValue: 0
                                toValue: 255
                                value: _frontend.baseColourGreen
                                onValueChanged: {
                                    _frontend.baseColourGreen = value
                                }
                            }
                            Slider {
                                id: baseBlue
                                fromValue: 0
                                toValue: 255
                                value: _frontend.baseColourBlue
                                onValueChanged: {
                                    _frontend.baseColourBlue = value
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}