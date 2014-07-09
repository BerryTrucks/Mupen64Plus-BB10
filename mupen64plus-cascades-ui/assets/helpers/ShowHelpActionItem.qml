import bb.cascades 1.0

ActionItem {
    property bool helpEnabled
    
    id: help
    title: {
        if(helpEnabled) {
            qsTr("Hide Help")
        }
        else {
            qsTr("Show Help")
        }
    }
    imageSource: "asset:///images/ic_help.png"
    ActionBar.placement: _frontend.playPlacement
    onTriggered: {
        if(!helpEnabled)
            helpEnabled = true;
        else
            helpEnabled = false;
    }
    
    onCreationCompleted: {
        if (_frontend.isOSThree) {
            imageSource = "asset:///images/ic_help3.png"
        }
    }
    
    shortcuts: [
        Shortcut {
            key: "h"
        }
    ]
}
