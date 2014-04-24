import bb.cascades 1.0

Sheet {
    id: aboutSheet
	Page {
        titleBar: TitleBar {
            title: qsTr("About")
            dismissAction: ActionItem {
                title: qsTr("Close")

                onTriggered: {
                    aboutSheet.close()
                }
            }
        }
        Container {
            leftPadding: 15.0
            topPadding: 15.0
            rightPadding: 15.0
            horizontalAlignment: HorizontalAlignment.Fill
            Container {
                layout: DockLayout {
                }
                horizontalAlignment: HorizontalAlignment.Fill
                Label {
                    text: qsTr("Version:")
                }
                Label {
                    text: _frontend.version
                    horizontalAlignment: HorizontalAlignment.Right
                }
            }
        }
	}
}