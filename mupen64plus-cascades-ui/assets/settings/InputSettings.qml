import bb.cascades 1.0
import "../helpers"

Page {
    actions: [
        ActionItem {
            title: qsTr("Set Defaults")
            imageSource: "asset:///images/default_icon.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (player1.selected) {
                    player1Settings.reset()
                }
                else if (player2.selected) {
                    player2Settings.reset()
                }
                else if (player3.selected) {
                    player3Settings.reset()
                }
                else if (player4.selected) {
                    player4Settings.reset()
                }
            }
        },
        ActionItem {
            title: qsTr("Clear")
            imageSource: "asset:///images/ic_clear.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
                if (player1.selected) {
                    player1Settings.clear()
                }
                else if (player2.selected) {
                    player2Settings.clear()
                }
                else if (player3.selected) {
                    player3Settings.clear()
                }
                else if (player4.selected) {
                    player4Settings.clear()
                }
            }
        }
    ]
    
    titleBar: TitleBar {
        id: titleBar4
        scrollBehavior: TitleBarScrollBehavior.Sticky
        kind: TitleBarKind.FreeForm
        
        kindProperties: FreeFormTitleBarKindProperties {
            id: titlebarProperties
            Container {
                layout: DockLayout {
                }
                
                leftPadding: 15.0
                rightPadding: 15.0
                Label {
                    text: qsTr("Input Settings")
                    textStyle.fontSize: FontSize.Large
                    textStyle.fontWeight: FontWeight.W500
                    
                    onCreationCompleted: {
                        if (!_frontend.isOSThree && _settings.Settings.Theme == 0) {
                            textStyle.color = Color.White
                        }
                    }
                    verticalAlignment: VerticalAlignment.Center
                }

                ImageView {
                    id: titleImage
                    verticalAlignment: VerticalAlignment.Center
                    horizontalAlignment: HorizontalAlignment.Right
                    imageSource: "asset:///images/input/kbd1.png"
                    
                    onCreationCompleted: {
                        if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                            imageSource = "asset:///images/input/kbd1_dk.png"
                        }
                    }
                }
                
                onTouch: {
                    if (event.touchType == TouchType.Up) {
                        titlebarProperties.expandableArea.expanded = !titlebarProperties.expandableArea.expanded
                    }
                }
            }
            
            expandableArea {
                content: Container {
                    preferredWidth: 768
                    horizontalAlignment: HorizontalAlignment.Center
                    background: _frontend.colorIndex == 0 ? Color.create("#E8E8E8") : Color.Black;
                    bottomPadding: 10
                    topPadding: 10
                    
                    SegmentedControl {
                        id: _player
                        Option {
                            id: player1
                            text: qsTr("Player 1")
                            selected: true
                        }
                        Option {
                            id: player2
                            text: qsTr("Player 2")
                        }
                        Option {
                            id: player3
                            text: qsTr("Player 3")
                        }
                        Option {
                            id: player4
                            text: qsTr("Player 4")
                        }
                        
                        onSelectedIndexChanged: {
                            resetImage()
                        }
                        
                        function resetImage1() {
                            if (player1Settings.controllerType == -4) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/gp1_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/gp1.png"
                                }
                            }
                            else if (player1Settings.controllerType == -3) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/ts1_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/ts1.png"
                                }
                            }
                            else if (player1Settings.controllerType == -5) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/tk1_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/tk1.png"
                                }
                            }
                            else {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/kbd1_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/kbd1.png"
                                }
                            }
                        }
                        
                        function resetImage2() {
                            if (player2Settings.controllerType == -4) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/gp2_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/gp2.png"
                                }
                            }
                            else if (player2Settings.controllerType == -3) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/ts2_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/ts2.png"
                                }
                            }
                            else if (player2Settings.controllerType == -5) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/tk2_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/tk2.png"
                                }
                            }
                            else {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/kbd2_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/kbd2.png"
                                }
                            }
                        }
                        
                        function resetImage3() {
                            if (player3Settings.controllerType == -4) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/gp3_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/gp3.png"
                                }
                            }
                            else if (player3Settings.controllerType == -3) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/ts3_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/ts3.png"
                                }
                            }
                            else if (player3Settings.controllerType == -5) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/tk3_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/tk3.png"
                                }
                            }
                            else {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/kbd3_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/kbd3.png"
                                }
                            }
                        }
                        
                        function resetImage4() {
                            if (player4Settings.controllerType == -4) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/gp4_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/gp4.png"
                                }
                            }
                            else if (player4Settings.controllerType == -3) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/ts4_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/ts4.png"
                                }
                            }
                            else if (player4Settings.controllerType == -5) {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/tk4_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/tk4.png"
                                }
                            }
                            else {
                                if (_frontend.isOSThree && _settings.Settings.Theme == 0) {
                                    titleImage.imageSource = "asset:///images/input/kbd4_dk.png"
                                }
                                else {
                                    titleImage.imageSource = "asset:///images/input/kbd4.png"
                                }
                            }
                        }
                        
                        function resetImage() {
                            if (selectedIndex == 0) {
                                resetImage1()
                            }
                            else if (selectedIndex == 1) {
                                resetImage2()
                            }
                            else if (selectedIndex == 2) {
                                resetImage3()
                            }
                            else {
                                resetImage4()
                            }
                            titlebarProperties.expandableArea.expanded = false
                        }
                    }
                }
                
                toggleArea: TitleBarExpandableAreaToggleArea.IndicatorOnly
                indicatorVisibility: TitleBarExpandableAreaIndicatorVisibility.Hidden
            }
        }
    }
    
    Container {
        topPadding: 15
        ControllerSettings {
            id: player1Settings
            player: 1
            visible: _player.selectedIndex == 0
            
            onControllerTypeChanged: {
                _player.resetImage1()
            }
        }
        ControllerSettings {
            id: player2Settings
            player: 2
            visible: _player.selectedIndex == 1
            
            onControllerTypeChanged: {
                _player.resetImage2()
            }
        }
        ControllerSettings {
            id: player3Settings
            player: 3
            visible: _player.selectedIndex == 2
            
            onControllerTypeChanged: {
                _player.resetImage3()
            }
        }
        ControllerSettings {
            id: player4Settings
            player: 4
            visible: _player.selectedIndex == 3
            
            onControllerTypeChanged: {
                _player.resetImage4()
            }
        }
    }
}
