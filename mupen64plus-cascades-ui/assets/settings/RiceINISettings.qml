import bb.cascades 1.0

Container {
    topPadding: 15.0

    Container {
        leftPadding: 15.0
        rightPadding: 15.0
        Label {
            id: nameDisplayLabel
            text: _frontend.RiceINI.CurrentRiceINI.Name
        }
        
        TextField {
            hintText: qsTr("ROM Name")
            
            function onUpdate() {
                visible = _frontend.RiceINI.CurrentRiceINI.Name.length == 0
                nameDisplayLabel.visible != visible
            }
            
            onTextChanged: {
                _frontend.RiceINI.CurrentRiceINI.Name = text
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }

        DropDown {
            title: qsTr("Accurate Texture Mapping")
            Option {
                text: qsTr("Not Set")
            }
            Option {
                text: "1"
            }
            Option {
                text: "2"
            }
            Option {
                text: "3"
            }
            
            function onUpdate() {
                selectedIndex = _frontend.RiceINI.CurrentRiceINI.AccurateTextureMapping
            }
            
            onSelectedIndexChanged: {
                _frontend.RiceINI.CurrentRiceINI.AccurateTextureMapping = selectedIndex
            }

            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        DropDown {
            title: qsTr("Fast Texture CRC")
            Option {
                text: qsTr("Not Set")
            }
            Option {
                text: "1"
            }
            Option {
                text: "2"
            }
            Option {
                text: "3"
            }
            
            function onUpdate() {
                selectedIndex = _frontend.RiceINI.CurrentRiceINI.FastTextureCRC
            }
            
            onSelectedIndexChanged: {
                _frontend.RiceINI.CurrentRiceINI.FastTextureCRC = selectedIndex
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        DropDown {
            title: qsTr("Normal Alpha Blender")
            Option {
                text: qsTr("Not Set")
            }
            Option {
                text: "1"
            }
            Option {
                text: "2"
            }
            Option {
                text: "3"
            }
            
            function onUpdate() {
                selectedIndex = _frontend.RiceINI.CurrentRiceINI.NormalBlender
            }
            
            onSelectedIndexChanged: {
                _frontend.RiceINI.CurrentRiceINI.NormalBlender = selectedIndex
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        DropDown {
            title: qsTr("Normal Color Blender")
            Option {
                text: qsTr("Not Set")
            }
            Option {
                text: "1"
            }
            Option {
                text: "2"
            }
            Option {
                text: "3"
            }

            function onUpdate() {
                selectedIndex = _frontend.RiceINI.CurrentRiceINI.NormalCombiner
            }
            
            onSelectedIndexChanged: {
                _frontend.RiceINI.CurrentRiceINI.NormalCombiner = selectedIndex
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        DropDown {
            title: qsTr("Frame Buffer Emulation")
            Option {
                text: qsTr("Not Set")
            }
            Option {
                text: qsTr("Ignore")
            }
            Option {
                text: qsTr("Basic")
            }
            Option {
                text: qsTr("Basic and Writeback")
            }
            Option {
                text: qsTr("Writeback and Reload")
            }
            Option {
                text: qsTr("Complete")
            }
            Option {
                text: qsTr("With Emulator")
            }
            Option {
                text: qsTr("Basic and With Emulator")
            }
            Option {
                text: qsTr("With Emulator Read Only")
            }
            Option {
                text: qsTr("With Emulator Write Only")
            }

            function onUpdate() {
                selectedIndex = _frontend.RiceINI.CurrentRiceINI.FrameBufferOption
            }
            
            onSelectedIndexChanged: {
                _frontend.RiceINI.CurrentRiceINI.FrameBufferOption = selectedIndex
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        DropDown {
            title: qsTr("Render to Texture")
            Option {
                text: qsTr("Not Set")
            }
            Option {
                text: qsTr("Ignore")
            }
            Option {
                text: qsTr("Normal")
            }
            Option {
                text: qsTr("Write Back")
            }
            Option {
                text: qsTr("Write Back and Reload")
            }
            
            function onUpdate() {
                selectedIndex = _frontend.RiceINI.CurrentRiceINI.RenderToTextureOption
            }
            
            onSelectedIndexChanged: {
                _frontend.RiceINI.CurrentRiceINI.RenderToTextureOption = selectedIndex
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        DropDown {
            title: qsTr("Screen Update")
            Option {
                text: qsTr("Not Set")
            }
            Option {
                text: qsTr("At VI Update")
            }
            Option {
                text: qsTr("At VI Change")
            }
            Option {
                text: qsTr("At CI Change")
            }
            Option {
                text: qsTr("At 1st CI Change")
            }
            Option {
                text: qsTr("At 1st Primitive")
            }
            Option {
                text: qsTr("Before Screen Clear")
            }
            Option {
                text: qsTr("At VI Update and Drawn")
            }
            
            function onUpdate() {
                selectedIndex = _frontend.RiceINI.CurrentRiceINI.ScreenUpdateSetting
            }
            
            onSelectedIndexChanged: {
                _frontend.RiceINI.CurrentRiceINI.ScreenUpdateSetting = selectedIndex
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        DropDown {
            title: qsTr("Use Full TMEM")
            Option {
                text: qsTr("Not Set")
            }
            Option {
                text: qsTr("Disable")
            }
            Option {
                text: qsTr("Enable")
            }
            
            function onUpdate() {
                selectedIndex = _frontend.RiceINI.CurrentRiceINI.FullTMEM
            }
            
            onSelectedIndexChanged: {
                _frontend.RiceINI.CurrentRiceINI.FullTMEM = selectedIndex
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        DropDown {
            title: qsTr("Use CI Width and Ratio")
            Option {
                text: qsTr("Not Set")
            }
            Option {
                text: qsTr("For NTSC")
            }
            Option {
                text: qsTr("For PAL")
            }
            
            function onUpdate() {
                selectedIndex = _frontend.RiceINI.CurrentRiceINI.UseCIWidthAndRatio
            }
            
            onSelectedIndexChanged: {
                _frontend.RiceINI.CurrentRiceINI.UseCIWidthAndRatio = selectedIndex
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Disable Texture CRC")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.DisableTextureCRC
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.DisableTextureCRC = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Disable Culling")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.DisableCulling
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.DisableCulling = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Primary Depth Hack")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.PrimaryDepthHack
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.PrimaryDepthHack = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Texture 1 Hack")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.Texture1Hack
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.Texture1Hack = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Fast Load Tile")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.FastLoadTile
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.FastLoadTile = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Use Smaller Texture")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.UseSmallerTexture
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.UseSmallerTexture = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Inc Tex Rect Edge")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.IncTexRectEdge
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.IncTexRectEdge = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Z Hack")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.ZHack
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.ZHack = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Tex Rect Scale Hack")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.TextureScaleHack
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.TextureScaleHack = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Alternative Txt Size Method")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.TxtSizeMethod2
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.TxtSizeMethod2 = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Enable Txt LOD")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.EnableTxtLOD
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.EnableTxtLOD = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Disable Obj BG")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.DisableObjBG
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.DisableObjBG = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Force Screen Clear")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.ForceScreenClear
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.ForceScreenClear = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Emulate Clear")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.EmulateClear
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.EmulateClear = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Disable Blender")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.DisableBlender
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.DisableBlender = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
        CheckBox {
            text: qsTr("Force Depth Buffer")

            function onUpdate() {
                checked = _frontend.RiceINI.CurrentRiceINI.ForceDepthBuffer
            }
            
            onCheckedChanged: {
                _frontend.RiceINI.CurrentRiceINI.ForceDepthBuffer = checked
            }
            
            onCreationCompleted: {
                onUpdate()
                _frontend.RiceINI.CurrentFileChanged.connect(onUpdate)
            }
        }
    }
}
