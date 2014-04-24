import bb.cascades 1.0

Sheet {
    id: glideHelpSheet
    peekEnabled: false

	Page {
        titleBar: TitleBar {
            title: qsTr("Glide 64 Compatibility")
            dismissAction: ActionItem {
                title: qsTr("Close")
                
                onTriggered: {
                    glideHelpSheet.close()
                }
            }
        }
        ScrollView {
	        WebView {
	            url: "local:///assets/html/Glide64compat.html"
	        }
	    }
    }
}