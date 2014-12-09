import bb.cascades 1.0

Sheet {
    id: rominfosheet

    Page {
        titleBar: TitleBar {
            title: qsTr("ROM Info")
            dismissAction: ActionItem {
                title: qsTr("Close")
                
                onTriggered: {
                    rominfosheet.close()
                }
            }
        }
        ScrollView {
            Container {
                Container {
                    leftPadding: 15.0
                    rightPadding: 15.0
                    topPadding: 20.0
                    horizontalAlignment: HorizontalAlignment.Fill
                    Container {
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: qsTr("<html><span style=\"text-decoration: underline;\">Good Name</span></html>")

                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: _frontend.info_goodname
                        }
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    leftPadding: 15.0
                    rightPadding: 15.0
                    Container {
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: qsTr("<html><span style=\"text-decoration: underline;\">Name</span></html>")
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: _frontend.info_name
                        }
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    leftPadding: 15.0
                    rightPadding: 15.0
                    Container {
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: qsTr("<html><span style=\"text-decoration: underline;\">MD5</span></html>")
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: _frontend.info_md5
                        }
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    leftPadding: 15.0
                    rightPadding: 15.0
                    Container {
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: qsTr("<html><span style=\"text-decoration: underline;\">CRC</span></html>")
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: _frontend.info_crc
                        }
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    leftPadding: 15.0
                    rightPadding: 15.0
                    Container {
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: qsTr("<html><span style=\"text-decoration: underline;\">Image Type</span></html>")
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: _frontend.info_imagetype
                        }
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    leftPadding: 15.0
                    rightPadding: 15.0
                    Container {
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: qsTr("<html><span style=\"text-decoration: underline;\">ROM Size</span></html>")
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: _frontend.info_romsize
                        }
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    leftPadding: 15.0
                    rightPadding: 15.0
                    Container {
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: qsTr("<html><span style=\"text-decoration: underline;\">Version</span></html>")
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: _frontend.info_version
                        }
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    leftPadding: 15.0
                    rightPadding: 15.0
                    Container {
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: qsTr("<html><span style=\"text-decoration: underline;\">Manufacturer</span></html>")
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: _frontend.info_manufacturer
                        }
                    }
                }
                Divider {
                }
                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    leftPadding: 15.0
                    rightPadding: 15.0
                    Container {
                        Label {
                            verticalAlignment: VerticalAlignment.Center
                            text: qsTr("<html><span style=\"text-decoration: underline;\">Country</span></html>")
                        }
                    }
                    Container {
                        horizontalAlignment: HorizontalAlignment.Fill
                        Label {
                            horizontalAlignment: HorizontalAlignment.Right
                            verticalAlignment: VerticalAlignment.Center
                            text: _frontend.info_country
                        }
                    }
                }
            }
        }
    }
}