import bb.cascades 1.0
import bb.system 1.0
import player 1.0
import "functions.js" as Global

Sheet {
    id: settingsRoot
    property int groupSettingIndent: 40
    property variant configuration: app.player.playback.configuration
    onOpened: {
        app.analytics.settings(1)
    }
    onClosed: {
        app.analytics.settings(0)
        settingsRoot.destroy()
    }
    Page {
        titleBar: PlayerTitleBar {
            title: qsTr("Configuration") + Retranslate.onLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Close") + Retranslate.onLanguageChanged
                onTriggered: {
                    settingsRoot.close();
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Apply") + Retranslate.onLanguageChanged
                onTriggered: {
                    app.player.playback.configure()
                    app.cache.maxSize = Math.round(maxCacheSize.value)
                    app.cache.maxFiles = Math.round(maxCacheSongs.value) 
                    settingsRoot.close();
                }
            } 
        }
        Container {
            layout: DockLayout {}
            RotoZoomer {}
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                ScrollView {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    VerticalContainer {
                        leftPadding: 20
                        rightPadding: leftPadding
                        topPadding: 20
                        bottomPadding: topPadding
                        GroupContainer {
                            topPadding: 20
                            bottomPadding: 40
                            leftPadding: 20
                            rightPadding: leftPadding
                            opacity: 0.85
                            BlackLabel {
                                text: qsTr("Application") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                textStyle {
                                    base: SystemDefaults.TextStyles.TitleText
                                    fontWeight: FontWeight.Bold
                                    color: Color.Black
                                }
                            }
                            Animation {
                                onTouch: {
                                    mouse.showMouse()
                                }
                            }
                            BlackLabel {
                                text: qsTr("Version: <b>%1</b>").arg(app.version) + Retranslate.onLanguageChanged
                                textFormat: TextFormat.Html
                            }
                            BlackLabel {
                                property int requestId
                                textFormat: TextFormat.Html
                                onCreationCompleted: {
                                    text = qsTr("Catalog songs: <b>%1</b>").arg("calculating...") + Retranslate.onLanguageChanged
                                    app.catalog.resultReady.connect(function(responseId, result) {
                                        if(responseId == requestId) {
                                            requestId = 0
                                            text = qsTr("Catalog songs: <b>%1</b>").arg(result) + Retranslate.onLanguageChanged
                                        }
                                    })
                                    requestId = app.catalog.songCountAsync()
                                }
                            }
                            BlackLabel {
                                id: personalSongCount
                                property int requestId
                                textFormat: TextFormat.Html
                                onCreationCompleted: {
                                    updateCount()
                                }
                                function updateCount() {
                                    text = qsTr("Personal songs: <b>%1</b>").arg("calculating...") + Retranslate.onLanguageChanged
                                    app.catalog.resultReady.connect(function(responseId, result) {
                                            if(responseId == requestId) {
                                                requestId = 0
                                                text = qsTr("Personal songs: <b>%1</b>").arg(result) + Retranslate.onLanguageChanged
                                            }
                                    })
                                    personalSongCount.requestId = app.catalog.personalSongCountAsync()
                                }
                            }
                            BlackLabel {
                                text: qsTr("<a href='http://www.kosenkov.ca/policy.html'>Privacy Policy</a>") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                textFormat: TextFormat.Html
                            }
                        }
                        GroupContainer {
                            topPadding: 20
                            bottomPadding: 40
                            leftPadding: 20
                            rightPadding: leftPadding
                            opacity: 0.85
                            Label {
                                text: qsTr("Cache") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                textStyle {
                                    base: SystemDefaults.TextStyles.TitleText
                                    fontWeight: FontWeight.Bold
                                    color: Color.Black
                                }
                            }
                            BlackLabel {
                                text: qsTr("Cached Songs: <b>%1</b>").arg(app.player.cache.currentFiles) + Retranslate.onLanguageChanged
                                textFormat: TextFormat.Html
                            }
                            BlackLabel {
                                text: qsTr("Used Cache Size: <b>%1</b>").arg(Global.getSizeKb(app.player.cache.currentSize)) + Retranslate.onLanguageChanged
                                textFormat: TextFormat.Html
                            }
                            BlackLabel {
                                text: qsTr("Maximum Songs: %1").arg(Math.round(maxCacheSongs.value)) + Retranslate.onLanguageChanged
                            }
                            VerticalContainer {
                                leftPadding: groupSettingIndent
                                rightPadding: leftPadding
                                Slider {
                                    id: maxCacheSongs
                                    fromValue: 5
                                    toValue: 500
                                    value: app.cache.maxFiles
                                }
                            }
                            BlackLabel {
                                text: qsTr("Maximum Cache Size: %1").arg(Global.getSizeKb(Math.round(maxCacheSize.value))) + Retranslate.onLanguageChanged
                            }
                            VerticalContainer {
                                leftPadding: groupSettingIndent
                                rightPadding: leftPadding
                                Slider {
                                    id: maxCacheSize
                                    fromValue: 5 * 1024 * 1024
                                    toValue: 400 * 1024 * 1024
                                    value: app.cache.maxSize
                                }
                            }
                            Button {
                                text: qsTr("Export Cache") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                topMargin: 40
                                bottomMargin: 40
                                enabled: app.cache.currentFiles > 0
                                onClicked: {
                                    confirmExportingSongCache.show()
                                }
                                attachedObjects: [
                                    SystemDialog {
                                        id: confirmExportingSongCache
                                        title: qsTr("Confirm") + Retranslate.onLanguageChanged
                                        body: qsTr("The song cache will now be exported to the 'ModPlayer' directory in your device downloads directory") + Retranslate.onLanguageChanged
                                        onFinished: {
                                            if (result != SystemUiResult.ConfirmButtonSelection)
                                                return;
                                            app.cache.progressUpdate.connect(function(percent, fileName) {
                                                progress.progress = percent
                                                progress.body = fileName
                                                progress.show()
                                            })
                                            progress.show()
                                            app.cache.exportCache("ModPlayer")
                                            progress.cancel()
                                            cacheExportToast.show()
                                        }
                                    },
                                    ProgressToast {
                                        id: progress
                                    },
                                    SystemToast {
                                        id: cacheExportToast
                                        body: qsTr("The song cache has been exported successfully to the 'ModPlayer' directory in your device downloads directory") + Retranslate.onLanguageChanged
                                    }
                                ]
                            }
                            Button {
                                text: qsTr("Purge Cache") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                topMargin: 40
                                bottomMargin: 40
                                enabled: app.cache.currentFiles > 0
                                onClicked: {
                                    confirmPurgingSongCache.show()
                                }
                                attachedObjects: [
                                    SystemDialog {
                                        id: confirmPurgingSongCache
                                        title: qsTr("Confirm") + Retranslate.onLanguageChanged
                                        body: qsTr("Confirm purging the song cache") + Retranslate.onLanguageChanged
                                        onFinished: {
                                            if (result == SystemUiResult.ConfirmButtonSelection) {
                                                app.analytics.purgeCache()
                                                app.cache.purge()
                                                cachePurgedToast.show()
                                            }
                                        }
                                    },
                                    SystemToast {
                                        id: cachePurgedToast
                                        body: qsTr("The song cache has been purged") + Retranslate.onLanguageChanged
                                    }
                                ]
                            }
                        }
                        GroupContainer {
                            topPadding: 20
                            bottomPadding: 40
                            leftPadding: 20
                            rightPadding: leftPadding
                            opacity: 0.85
                            Label {
                                text: qsTr("Personal") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                textStyle {
                                    base: SystemDefaults.TextStyles.TitleText
                                    fontWeight: FontWeight.Bold
                                    color: Color.Black
                                }
                            }
                            Button {
                                text: qsTr("Reset Play Counts") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                topMargin: 40
                                bottomMargin: 40
                                onClicked: {
                                    confirmResetPlayCounts.show()
                                }
                                attachedObjects: [
                                    SystemDialog {
                                        id: confirmResetPlayCounts
                                        title: qsTr("Confirm") + Retranslate.onLanguageChanged
                                        body: qsTr("Confirm resetting song play counts") + Retranslate.onLanguageChanged
                                        onFinished: {
                                            if (result == SystemUiResult.ConfirmButtonSelection) {
                                                app.catalog.resetPlayCounts()
                                                songPlayCountsToast.show()
                                            }
                                        }
                                    },
                                    SystemToast {
                                        id: songPlayCountsToast
                                        body: qsTr("Song play counts have been reset") + Retranslate.onLanguageChanged
                                    }
                                ]
                            }
                            Button {
                                text: qsTr("Reset My Favourites") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                topMargin: 40
                                bottomMargin: 40
                                onClicked: {
                                    confirmMyFavouritesReset.show()
                                }
                                attachedObjects: [
                                    SystemDialog {
                                        id: confirmMyFavouritesReset
                                        title: qsTr("Confirm") + Retranslate.onLanguageChanged
                                        body: qsTr("Confirm resetting my favoruites list") + Retranslate.onLanguageChanged
                                        onFinished: {
                                            if (result == SystemUiResult.ConfirmButtonSelection) {
                                                app.catalog.resetMyFavourites();
                                                myFavouritesResetToast.show()
                                            }
                                        }
                                    },
                                    SystemToast {
                                        id: myFavouritesResetToast
                                        body: qsTr("My favourites list is reset") + Retranslate.onLanguageChanged
                                    }
                                ]
                            }
                            Button {
                                text: qsTr("Import My Songs") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                topMargin: 40
                                bottomMargin: 40
                                onClicked: {
                                    confirmImport.show()
                                }
                                attachedObjects: [
                                    SystemDialog {
                                        id: confirmImport
                                        title: qsTr("Confirm") + Retranslate.onLanguageChanged
                                        body: qsTr("Would you like to import local songs and playlists?") + Retranslate.onLanguageChanged
                                        onFinished: {
                                            if (result == SystemUiResult.ConfirmButtonSelection) {
                                                app.player.importSongs()
                                                personalSongCount.updateCount()
                                            }
                                        }
                                    }
                                ]
                            }
                        }
                        GroupContainer {
                            topPadding: 20
                            bottomPadding: 40
                            leftPadding: 20
                            rightPadding: leftPadding
                            opacity: 0.85
                            Label {
                                text: qsTr("Background") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                textStyle {
                                    base: SystemDefaults.TextStyles.TitleText
                                    fontWeight: FontWeight.Bold
                                    color: Color.Black
                                }
                            }
                            CheckBox {
                                id: animationEnabled
                                text: qsTr("Enable Background Animation") + Retranslate.onLanguageChanged
                                checked: settingsRoot.configuration.animationEnabled
                                onCheckedChanged: {
                                    settingsRoot.configuration.animationEnabled = animationEnabled.checked
                                }
                            }
                            DropDown {
                                id: wallpaper
                                title: qsTr("Background") + Retranslate.onLanguageChanged
                                property variant allWallpapers: [
                                    {
                                        name: "ModPlayer Classic",
                                        path:"asset:///images/backgrounds/view_back.amd", 
                                        repeatable: true, 
                                        animatable: true
                                    },
                                    {
                                        name: "Purple",
                                        path:"asset:///images/backgrounds/purple.jpg",
                                        repeatable: false,
                                        animatable: false
                                    },
                                    {
                                        name: "Digital",
                                        path:"asset:///images/backgrounds/digital.jpg",
                                        repeatable: false,
                                        animatable: true
                                    },
                                    {
                                        name: "Flower",
                                        path:"asset:///images/backgrounds/flower.jpg",
                                        repeatable: false,
                                        animatable: false
                                    },
                                    {
                                        name: "Cork",
                                        path:"asset:///images/backgrounds/cork.amd",
                                        repeatable: true,
                                        animatable: false
                                    },
                                    {
                                        name: "Asphalt",
                                        path:"asset:///images/backgrounds/asphalt.amd",
                                        repeatable: true,
                                        animatable: true
                                    },
                                    {
                                        name: "Amiga",
                                        path:"asset:///images/backgrounds/amiga.jpg",
                                        repeatable: false,
                                        animatable: false
                                    },
                                    {
                                        name: "White",
                                        path:"asset:///images/backgrounds/white.amd",
                                        repeatable: true,
                                        animatable: false
                                    },
                                    {
                                        name: "Black",
                                        path:"asset:///images/backgrounds/black.amd",
                                        repeatable: true,
                                        animatable: false
                                    },
                                    {
                                        name: "Green",
                                        path:"asset:///images/backgrounds/green.amd",
                                        repeatable: false,
                                        animatable: false
                                    },
                                    {
                                        name: "Blue",
                                        path:"asset:///images/backgrounds/blue.jpg",
                                        repeatable: false,
                                        animatable: false
                                    },
                                    {
                                        name: "ModPlayer",
                                        path:"asset:///images/backgrounds/modplayer.amd",
                                        repeatable: true,
                                        animatable: true
                                    },
                                    {
                                        name: "Abstract",
                                        path:"asset:///images/backgrounds/abstract.jpg",
                                        repeatable: false,
                                        animatable: false
                                    },
                                    {
                                        name: "Cloth",
                                        path:"asset:///images/backgrounds/cloth.amd",
                                        repeatable: true,
                                        animatable: true
                                    }
                                ]
                                attachedObjects: [
                                    ComponentDefinition {
                                        id: optionDefinition
                                        Option {}
                                    }
                                ]
                                onCreationCompleted: {
                                    var selectedName = app.wallpaper.name
                                    for(var i = 0; i < allWallpapers.length; ++i) {
                                        var newOption = optionDefinition.createObject()
                                        newOption.text = allWallpapers[i].name
                                        newOption.value = i
                                        if(selectedName == allWallpapers[i].name) {
                                            newOption.selected = true
                                        }
                                        add(newOption)
                                    }
                                }
                                onSelectedValueChanged: {
                                    app.wallpaper.name = selectedOption.text
                                    app.wallpaper.path = allWallpapers[selectedValue].path
                                    app.wallpaper.repeatable = allWallpapers[selectedValue].repeatable
                                    app.wallpaper.animatable = allWallpapers[selectedValue].animatable
                                }
                            }
                        }
                        GroupContainer {
                            topPadding: 20
                            bottomPadding: 40
                            leftPadding: 20
                            rightPadding: leftPadding
                            opacity: 0.85
                            Label {
                                text: qsTr("Mixer Configuration") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                textStyle {
                                    base: SystemDefaults.TextStyles.TitleText
                                    fontWeight: FontWeight.Bold
                                    color: Color.Black
                                }
                            }
                            DropDown {
                                id: output
                                title: qsTr("Output") + Retranslate.onLanguageChanged
                                Option {
                                    text: qsTr("Stereo") + Retranslate.onLanguageChanged
                                    value: 1
                                    selected: settingsRoot.configuration.stereo == true 
                                }
                                Option {
                                    text: qsTr("Mono") + Retranslate.onLanguageChanged
                                    value: 0
                                    selected: settingsRoot.configuration.stereo == false 
                                }
                                
                                onSelectedValueChanged: {
                                    settingsRoot.configuration.stereo = (selectedValue == 1)
                                }
                            }
                            DropDown {
                                id: bitsPerSample
                                title: qsTr("Bits per Sample") + Retranslate.onLanguageChanged
                                Option {
                                    text: "8"
                                    value: 8
                                    selected: settingsRoot.configuration.sampleSize == value
                                }
                                Option {
                                    text: "16"
                                    value: 16
                                    selected: settingsRoot.configuration.sampleSize == value
                                }
                                Option {
                                    text: "32"
                                    value: 32
                                    selected: settingsRoot.configuration.sampleSize == value
                                }
                                
                                onSelectedValueChanged: {
                                    settingsRoot.configuration.sampleSize = selectedValue
                                }
                            }
                            DropDown {
                                id: frequency
                                title: qsTr("Frequency") + Retranslate.onLanguageChanged
                                Option {
                                    text: "44100 Hz"
                                    description: qsTr("44100 Hz sampling rate") + Retranslate.onLanguageChanged
                                    value: 44100
                                    selected: settingsRoot.configuration.frequency == value
                                }
                                Option {
                                    text: "22050 Hz"
                                    description: qsTr("22050 Hz sampling rate") + Retranslate.onLanguageChanged
                                    value: 22050
                                    selected: settingsRoot.configuration.frequency == value
                                }
                                Option {
                                    text: "11025 Hz"
                                    description: qsTr("11025 Hz sampling rate") + Retranslate.onLanguageChanged
                                    value: 11025
                                    selected: settingsRoot.configuration.frequency == value
                                }
                                onSelectedValueChanged: {
                                    settingsRoot.configuration.frequency = selectedValue
                                }
                            }
                            DropDown {
                                id: resampling
                                title: qsTr("Resampling") + Retranslate.onLanguageChanged
                                Option {
                                    text: qsTr("Nearest") + Retranslate.onLanguageChanged
                                    description: qsTr("No interpolation (very fast)") + Retranslate.onLanguageChanged
                                    value: 0
                                    selected: settingsRoot.configuration.resamplingMode == value
                                }
                                Option {
                                    text: qsTr("Linear") + Retranslate.onLanguageChanged
                                    description: qsTr("Linear interpolation (fast, good quality)") + Retranslate.onLanguageChanged
                                    value: 1
                                    selected: settingsRoot.configuration.resamplingMode == value
                                }
                                Option {
                                    text: qsTr("Spline") + Retranslate.onLanguageChanged
                                    description: qsTr("Cubic spline interpolation (high quality)") + Retranslate.onLanguageChanged
                                    value: 2
                                    selected: settingsRoot.configuration.resamplingMode == value
                                }
                                Option {
                                    text: qsTr("FIR") + Retranslate.onLanguageChanged
                                    description: qsTr("8-tap fir filter (extremely high quality)") + Retranslate.onLanguageChanged
                                    value: 3
                                    selected: settingsRoot.configuration.resamplingMode == value
                                }
                                onSelectedValueChanged: {
                                    settingsRoot.configuration.resamplingMode = selectedValue
                                }
                            }
                        }
                        GroupContainer {
                            topPadding: 20
                            bottomPadding: 40
                            leftPadding: 20
                            rightPadding: leftPadding
                            opacity: 0.85
                            Label {
                                text: qsTr("Master Volume") + Retranslate.onLanguageChanged
                                textStyle {
                                    fontSize: FontSize.Large
                                    color: Color.Black
                                }
                            }
                            VerticalContainer {
                                leftPadding: groupSettingIndent
                                rightPadding: leftPadding
                                Slider {
                                    id: masterVolume
                                    fromValue: 1
                                    toValue: 512
                                    value: settingsRoot.configuration.masterVolume
                                    onValueChanged: {
                                        app.player.playback.currentSong.masterVolume = masterVolume.value
                                        settingsRoot.configuration.masterVolume = masterVolume.value
                                    }
                                }
                            }
                            Label {
                                text: qsTr("Stereo Separation") + Retranslate.onLanguageChanged
                                textStyle {
                                    fontSize: FontSize.Large
                                    color: Color.Black
                                }
                            }
                            VerticalContainer {
                                leftPadding: groupSettingIndent
                                rightPadding: leftPadding
                                Slider {
                                    id: stereoSeparation
                                    fromValue: 1
                                    toValue: 256
                                    value: settingsRoot.configuration.stereoSeparation
                                    onValueChanged: {
                                        settingsRoot.configuration.stereoSeparation = stereoSeparation.value
                                    }
                                }
                            }
                            CheckBox {
                                id: oversampling
                                text: qsTr("Enable Oversampling") + Retranslate.onLanguageChanged
                                checked: settingsRoot.configuration.oversamplingEnabled
                                onCheckedChanged: {
                                    settingsRoot.configuration.oversamplingEnabled = oversampling.checked
                                }
                            }
                            CheckBox {
                                id: noiseReduction
                                text: qsTr("Enable Noise Reduction") + Retranslate.onLanguageChanged
                                checked: settingsRoot.configuration.noiseReductionEnabled
                                onCheckedChanged: {
                                    settingsRoot.configuration.noiseReductionEnabled = noiseReduction.checked
                                }
                            }
                        }
                        GroupContainer {
                            topPadding: 20
                            bottomPadding: 40
                            leftPadding: 20
                            rightPadding: leftPadding
                            opacity: 0.85
                            CheckBox {
                                id: reverbEnabled
                                text: qsTr("Enable Reverb") + Retranslate.onLanguageChanged
                                checked: settingsRoot.configuration.reverbEnabled
                                onCheckedChanged: {
                                    settingsRoot.configuration.reverbEnabled = reverbEnabled.checked
                                }
                            }
                            VerticalContainer {
                                leftPadding: groupSettingIndent
                                rightPadding: leftPadding
                                Label {
                                    text: qsTr("Reverb Depth") + Retranslate.onLanguageChanged
                                    visible: reverbEnabled.checked
                                    textStyle {
                                        fontStyle: FontStyle.Italic
                                        fontWeight: FontWeight.W100
                                        color: Color.Black
                                    }
                                }
                                Slider {
                                    id: reverbLevel
                                    visible: reverbEnabled.checked
                                    fromValue: 0
                                    toValue: 100
                                    value: settingsRoot.configuration.reverbLevel
                                    onValueChanged: {
                                        settingsRoot.configuration.reverbLevel = reverbLevel.value
                                    }
                                }
                                Label {
                                    text: qsTr("Reverb Delay") + Retranslate.onLanguageChanged
                                    visible: reverbEnabled.checked
                                    textStyle {
                                        fontStyle: FontStyle.Italic
                                        fontWeight: FontWeight.W100
                                        color: Color.Black
                                    }
                                }
                                Slider {
                                    id: reverbDelay
                                    visible: reverbEnabled.checked
                                    fromValue: 40
                                    toValue: 200
                                    value: settingsRoot.configuration.reverbDelay
                                    onValueChanged: {
                                        settingsRoot.configuration.reverbDelay = reverbDelay.value
                                    }
                                }
                            }
                        }
                        GroupContainer {
                            topPadding: 20
                            bottomPadding: 40
                            leftPadding: 20
                            rightPadding: leftPadding
                            opacity: 0.85
                            CheckBox {
                                id: megabassEnabled
                                text: qsTr("Enable MegaBass") + Retranslate.onLanguageChanged
                                checked: settingsRoot.configuration.bassEnabled
                                onCheckedChanged: {
                                    settingsRoot.configuration.bassEnabled = megabassEnabled.checked
                                }
                            }
                            VerticalContainer {
                                leftPadding: groupSettingIndent
                                rightPadding: leftPadding
                                Label {
                                    text: qsTr("Bass Amount") + Retranslate.onLanguageChanged
                                    visible: megabassEnabled.checked
                                    textStyle {
                                        fontStyle: FontStyle.Italic
                                        fontWeight: FontWeight.W100
                                        color: Color.Black
                                    }
                                }
                                Slider {
                                    id: megabassLevel
                                    visible: megabassEnabled.checked
                                    fromValue: 0
                                    toValue: 100
                                    value: settingsRoot.configuration.bassLevel
                                    onValueChanged: {
                                        settingsRoot.configuration.bassLevel = megabassLevel.value
                                    }
                                }
                                Label {
                                    text: qsTr("Bass Cutoff") + Retranslate.onLanguageChanged
                                    visible: megabassEnabled.checked
                                    textStyle {
                                        fontStyle: FontStyle.Italic
                                        fontWeight: FontWeight.W100
                                        color: Color.Black
                                    }
                                }
                                Slider {
                                    id: megabassCutoff
                                    visible: megabassEnabled.checked
                                    fromValue: 10
                                    toValue: 100
                                    value: settingsRoot.configuration.bassCutOff
                                    onValueChanged: {
                                        settingsRoot.configuration.bassCutOff = megabassCutoff.value
                                    }
                                }
                            }
                        }
                        GroupContainer {
                            topPadding: 20
                            bottomPadding: 40
                            leftPadding: 20
                            rightPadding: leftPadding
                            opacity: 0.85
                            CheckBox {
                                id: surroundEnabled
                                text: qsTr("Enable Surround Sound") + Retranslate.onLanguageChanged
                                checked: settingsRoot.configuration.surroundEnabled
                                onCheckedChanged: {
                                    settingsRoot.configuration.surroundEnabled = surroundEnabled.checked
                                }
                            }
                            VerticalContainer {
                                leftPadding: groupSettingIndent
                                rightPadding: leftPadding
                                Label {
                                    text: qsTr("Surrond Depth") + Retranslate.onLanguageChanged
                                    visible: surroundEnabled.checked
                                    textStyle {
                                        fontStyle: FontStyle.Italic
                                        fontWeight: FontWeight.W100
                                        color: Color.Black
                                    }
                                }
                                Slider {
                                    id: surroundLevel
                                    visible: surroundEnabled.checked
                                    fromValue: 0
                                    toValue: 100
                                    value: settingsRoot.configuration.surroundLevel
                                    onValueChanged: {
                                        settingsRoot.configuration.surroundLevel = surroundLevel.value
                                    }
                                }
                                Label {
                                    text: qsTr("Surrond Delay") + Retranslate.onLanguageChanged
                                    visible: surroundEnabled.checked
                                    textStyle {
                                        fontStyle: FontStyle.Italic
                                        fontWeight: FontWeight.W100
                                        color: Color.Black
                                    }
                                }
                                Slider {
                                    id: surroundDelay
                                    visible: surroundEnabled.checked
                                    fromValue: 5
                                    toValue: 50
                                    value: settingsRoot.configuration.surroundDelay
                                    onValueChanged: {
                                        settingsRoot.configuration.surroundDelay = surroundDelay.value
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Mouse {
                id: mouse
            }
        }
    }
}