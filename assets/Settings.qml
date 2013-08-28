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
        titleBar: TitleBar {
            title: qsTr("Configuration")
            visibility: ChromeVisibility.Visible
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    settingsRoot.close();
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Apply")
                onTriggered: {
                    app.player.playback.configure()
                    app.cache.maxSize = Math.round(maxCacheSize.value)
                    app.cache.maxFiles = Math.round(maxCacheSongs.value) 
                    settingsRoot.close();
                }
            } 
        }
        ViewContainer {
            ScrollView {
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
                        Label {
                            text: qsTr("Cache")
                            horizontalAlignment: HorizontalAlignment.Center
                            textStyle.base: SystemDefaults.TextStyles.TitleText
                            textStyle.fontWeight: FontWeight.Bold
                            textStyle.color: Color.Black
                        }
                        Label {
                            text: qsTr("Cached Songs: <b>%1</b>").arg(app.player.cache.currentFiles)
                            textFormat: TextFormat.Html
                            textStyle.color: Color.Black
                        }
                        Label {
                            text: qsTr("Used Cache Size: <b>%1</b>").arg(Global.getSizeKb(app.player.cache.currentSize))
                            textFormat: TextFormat.Html
                            textStyle.color: Color.Black
                        }
                        
                        Label {
                            text: qsTr("Maximum Songs: %1").arg(Math.round(maxCacheSongs.value))
                            textStyle.color: Color.Black
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
                        Label {
                            text: qsTr("Maximum Cache Size: %1").arg(Global.getSizeKb(Math.round(maxCacheSize.value)))
                            textStyle.color: Color.Black 
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
                            text: qsTr("Purge cache")
                            horizontalAlignment: HorizontalAlignment.Center
                            topMargin: 40
                            bottomMargin: 40
                            onClicked: {
                                confirmPurgingSongCache.show()
                            }
                            attachedObjects: [
                                SystemDialog {
                                    id: confirmPurgingSongCache
                                    title: qsTr("Confirm")
                                    body: qsTr("Confirm purging the song cache")
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
                                    body: qsTr("The song cache has been purged")
                                }
                            ]
                        }
                    }
                    GroupContainer {
                        topPadding: 20
                        bottomPadding: 40
                        leftPadding: 20
                        rightPadding: leftPadding
                        Label {
                            text: qsTr("Personal")
                            horizontalAlignment: HorizontalAlignment.Center
                            textStyle.base: SystemDefaults.TextStyles.TitleText
                            textStyle.fontWeight: FontWeight.Bold
                            textStyle.color: Color.Black
                        }
                        
                        Button {
                            text: qsTr("Reset Play Counts")
                            horizontalAlignment: HorizontalAlignment.Center
                            topMargin: 40
                            bottomMargin: 40
                            onClicked: {
                                confirmResetPlayCounts.show()
                            }
                            attachedObjects: [
                                SystemDialog {
                                    id: confirmResetPlayCounts
                                    title: qsTr("Confirm")
                                    body: qsTr("Confirm resetting song play counts")
                                    onFinished: {
                                        if (result == SystemUiResult.ConfirmButtonSelection) {
                                            app.catalog.resetPlayCounts()
                                            songPlayCountsToast.show()
                                        }
                                    }
                                },
                                SystemToast {
                                    id: songPlayCountsToast
                                    body: qsTr("Song play counts have been reset")
                                }
                            ]
                        }
                        
                        Button {
                            text: qsTr("Reset My Favourites")
                            horizontalAlignment: HorizontalAlignment.Center
                            topMargin: 40
                            bottomMargin: 40
                            onClicked: {
                                confirmMyFavouritesReset.show()
                            }
                            attachedObjects: [
                                SystemDialog {
                                    id: confirmMyFavouritesReset
                                    title: qsTr("Confirm")
                                    body: qsTr("Confirm resetting my favoruites list")
                                    onFinished: {
                                        if (result == SystemUiResult.ConfirmButtonSelection) {
                                            app.catalog.resetMyFavourites();
                                            myFavouritesResetToast.show()
                                        }
                                    }
                                },
                                SystemToast {
                                    id: myFavouritesResetToast
                                    body: qsTr("My favourites list is reset")
                                }
                            ]
                        }
                    }
                    GroupContainer {
                        topPadding: 20
                        bottomPadding: 40
                        leftPadding: 20
                        rightPadding: leftPadding
                        Label {
                            text: qsTr("Mixer Configuration")
                            horizontalAlignment: HorizontalAlignment.Center
                            textStyle.base: SystemDefaults.TextStyles.TitleText
                            textStyle.fontWeight: FontWeight.Bold
                            textStyle.color: Color.Black
                        }
                        DropDown {
                            id: output
                            title: qsTr("Output")
                            Option {
                                text: qsTr("Stereo")
                                value: 1
                                selected: settingsRoot.configuration.stereo == true 
                            }
                            Option {
                                text: qsTr("Mono")
                                value: 0
                                selected: settingsRoot.configuration.stereo == false 
                            }
                            
                            onSelectedValueChanged: {
                                settingsRoot.configuration.stereo = (selectedValue == 1)
                            }
                        }
                        DropDown {
                            id: bitsPerSample
                            title: qsTr("Bits per Sample")
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
                            title: qsTr("Frequency")
                            Option {
                                text: "44100 Hz"
                                description: qsTr("44100 Hz sampling rate")
                                value: 44100
                                selected: settingsRoot.configuration.frequency == value
                            }
                            Option {
                                text: "22050 Hz"
                                description: qsTr("22050 Hz sampling rate")
                                value: 22050
                                selected: settingsRoot.configuration.frequency == value
                            }
                            Option {
                                text: "11025 Hz"
                                description: qsTr("11025 Hz sampling rate")
                                value: 11025
                                selected: settingsRoot.configuration.frequency == value
                            }
                            onSelectedValueChanged: {
                                settingsRoot.configuration.frequency = selectedValue
                            }
                        }
                        DropDown {
                            id: resampling
                            title: qsTr("Resampling")
                            Option {
                                text: qsTr("Nearest")
                                description: qsTr("No interpolation (very fast)")
                                value: 0
                                selected: settingsRoot.configuration.resamplingMode == value
                            }
                            Option {
                                text: qsTr("Liner")
                                description: qsTr("Linear interpolation (fast, good quality)")
                                value: 1
                                selected: settingsRoot.configuration.resamplingMode == value
                            }
                            Option {
                                text: qsTr("Spline")
                                description: qsTr("Cubic spline interpolation (high quality)")
                                value: 2
                                selected: settingsRoot.configuration.resamplingMode == value
                            }
                            Option {
                                text: qsTr("FIR")
                                description: qsTr("8-tap fir filter (extremely high quality)")
                                value: 3
                                selected: settingsRoot.configuration.resamplingMode == value
                            }
                            onSelectedValueChanged: {
                                settingsRoot.configuration.resamplingMode = selectedValue
                            }
                        }
                        Label {
                            text: qsTr("Stereo Separation")
                            textStyle.fontSize: FontSize.Large
                            textStyle.color: Color.Black
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
                            text: qsTr("Enable Oversampling")
                            checked: settingsRoot.configuration.oversamplingEnabled
                            onCheckedChanged: {
                                settingsRoot.configuration.oversamplingEnabled = oversampling.checked
                            }
                        }
                        CheckBox {
                            id: noiseReduction
                            text: qsTr("Enable Noise Reduction")
                            checked: settingsRoot.configuration.noiseReductionEnabled
                            onCheckedChanged: {
                                settingsRoot.configuration.noiseReductionEnabled = noiseReduction.checked
                            }
                        }
                        CheckBox {
                            id: reverbEnabled
                            text: qsTr("Enable Reverb")
                            checked: settingsRoot.configuration.reverbEnabled
                            onCheckedChanged: {
                                settingsRoot.configuration.reverbEnabled = reverbEnabled.checked
                            }
                        }
                        VerticalContainer {
                            leftPadding: groupSettingIndent
                            rightPadding: leftPadding
                            Label {
                                text: qsTr("Reverb Depth")
                                visible: reverbEnabled.checked
                                textStyle.fontStyle: FontStyle.Italic
                                textStyle.fontWeight: FontWeight.W100
                                textStyle.color: Color.Black
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
                                text: qsTr("Reverb Delay")
                                visible: reverbEnabled.checked
                                textStyle.fontStyle: FontStyle.Italic
                                textStyle.fontWeight: FontWeight.W100
                                textStyle.color: Color.Black
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
                        CheckBox {
                            id: megabassEnabled
                            text: qsTr("Enable MegaBass")
                            checked: settingsRoot.configuration.bassEnabled
                            onCheckedChanged: {
                                settingsRoot.configuration.bassEnabled = megabassEnabled.checked
                            }
                        }
                        VerticalContainer {
                            leftPadding: groupSettingIndent
                            rightPadding: leftPadding
                            Label {
                                text: qsTr("Bass Amount")
                                visible: megabassEnabled.checked
                                textStyle.fontStyle: FontStyle.Italic
                                textStyle.fontWeight: FontWeight.W100
                                textStyle.color: Color.Black
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
                                text: qsTr("Bass Cutoff")
                                visible: megabassEnabled.checked
                                textStyle.fontStyle: FontStyle.Italic
                                textStyle.fontWeight: FontWeight.W100
                                textStyle.color: Color.Black
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
                        CheckBox {
                            id: surroundEnabled
                            text: qsTr("Enable Surround Sound")
                            checked: settingsRoot.configuration.surroundEnabled
                            onCheckedChanged: {
                                settingsRoot.configuration.surroundEnabled = surroundEnabled.checked
                            }
                        }
                        VerticalContainer {
                            leftPadding: groupSettingIndent
                            rightPadding: leftPadding
                            Label {
                                text: qsTr("Surrond Depth")
                                visible: surroundEnabled.checked
                                textStyle.fontStyle: FontStyle.Italic
                                textStyle.fontWeight: FontWeight.W100
                                textStyle.color: Color.Black
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
                                text: qsTr("Surrond Delay")
                                visible: surroundEnabled.checked
                                textStyle.fontStyle: FontStyle.Italic
                                textStyle.fontWeight: FontWeight.W100
                                textStyle.color: Color.Black
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
    }
}