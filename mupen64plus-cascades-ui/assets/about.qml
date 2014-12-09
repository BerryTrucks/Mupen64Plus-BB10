import bb.cascades 1.0
import "helpers"

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
        ScrollView {
            Container {
                Header {
                    title: qsTr("Version")
                }
                AboutItem {
                    title: qsTr("Mupen64+ BB10 Version:")
                    value: _frontend.version
                }
                AboutItem {
                    title: qsTr("Mupen64+ Core Version:")
                    value: "2.0.0"
                }
                AboutItem {
                    title: qsTr("GLES2rice Version:")
                    value: "2.0.0"
                }
                AboutItem {
                    title: qsTr("GLES2n64 Version:")
                    value: "0.0.5"
                }
                AboutItem {
                    title: qsTr("GLES2glide Version:")
                    value: "2.0.0"
                }
                AboutItem {
                    title: qsTr("Audio SDL Version:")
                    value: "2.0.0"
                }
                AboutItem {
                    title: qsTr("Reality Signal Processor Version:")
                    value: "2.0.0"
                    bottomPadding: 15.0
                }
                Header {
                    title: qsTr("API Version")
                }
                AboutItem {
                    title: qsTr("Config API Version:")
                    value: "2.3.0"
                }
                AboutItem {
                    title: qsTr("Video Extension API Version:")
                    value: "3.0.0"
                }
                AboutItem {
                    title: qsTr("Video API Version:")
                    value: "2.2.0"
                }
                AboutItem {
                    title: qsTr("Audio API Version:")
                    value: "2.0.0"
                }
                AboutItem {
                    title: qsTr("Input API Version:")
                    value: "2.0.1"
                }
                AboutItem {
                    title: qsTr("RSP API Version:")
                    value: "2.0.0"
                    bottomPadding: 15.0
                }
                Header {
                    title: qsTr("Compile Info")
                }
                AboutItem {
                     title: qsTr("Compile Date:")
                     value: _frontend.compileDate
                }
                AboutItem {
                     title: qsTr("Compiled By:")
                     value: "Travis Redpath"
                }
            }
        }
    }
}