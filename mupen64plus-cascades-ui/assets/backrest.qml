import bb.cascades 1.0

Sheet {
    id: root
    property bool restoring: false
    property bool restored: false
    property bool backing: false
    
    Page {
        titleBar: TitleBar {
            dismissAction: ActionItem {
                title: qsTr("Cancel")
                enabled: !restored && !restoring && !backing
                
                onTriggered: {
                    root.close()
                }
            }

            scrollBehavior: TitleBarScrollBehavior.Sticky
            title: qsTr("Backup/Restore")
        }
        Container {
            layout: DockLayout {
            }

            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                visible: ! restored && ! restoring

                Container {
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 15.0
                    leftPadding: 15.0
                    rightPadding: 15.0
                    Button {
                        horizontalAlignment: HorizontalAlignment.Fill
                        text: qsTr("Backup")

                        onClicked: {
                            backing = true
                            _frontend.backup()
                        }
                    }
                }
                Container {
                    topPadding: 15.0
                    leftPadding: 15.0
                    rightPadding: 15.0
                    horizontalAlignment: HorizontalAlignment.Fill
                    Button {
                        horizontalAlignment: HorizontalAlignment.Fill
                        text: qsTr("Restore")

                        onClicked: {
                            restoring = true
                            _frontend.restore()
                        }
                    }
                }
                Container {
                    leftPadding: 15.0
                    topPadding: 15.0
                    rightPadding: 15.0
                    Label {
                        multiline: true
                        text: qsTr("Backup/Restore app settings and save game files.")

                    }

                }

            }
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                background: Color.create("#7d000000")
                visible: restored || restoring || backing
                layout: DockLayout {
                }
                ActivityIndicator {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    minWidth: 200.0
                    minHeight: 200.0
                    running: restoring || backing
                }
                Label {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    textStyle.fontSize: FontSize.Large
                    textStyle.color: Color.White
                    text: qsTr("<html><body>Restoration complete:<br/><font style=\"color:red\"><b>Restart</b> Mupen64Plus BB</font></body></html>")
                    multiline: true
                    textStyle.textAlign: TextAlign.Center
                    visible: restored
                }
            }
        }
    }
    
    function onBackupComplete() {
        backing = false
    }
    
    function onRestoreComplete() {
        restoring = false
        restored = true
    }
    
    function onRestoreCanceled() {
        restoring = false
        restored = false
    }
    
    onCreationCompleted: {
        _frontend.restoreComplete.connect(onRestoreComplete)
        _frontend.backupComplete.connect(onBackupComplete)
        _frontend.restoreCanceled.connect(onRestoreCanceled)
    }
}