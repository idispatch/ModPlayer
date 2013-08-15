import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Sheet {
    id: settingsRoot
    property int groupSettingIndent: 50
    property variant configuration: app.player.playback.configuration

    onClosed: {
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

        ScrollView {
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }

                leftPadding: 30
                rightPadding: 30
                topPadding: 20
                bottomPadding: 20

                Label {
                    text: qsTr("Cache")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    textStyle.fontWeight: FontWeight.Bold
                }
                Label {
                    text: "Cached Songs: <b>" + app.player.cache.currentFiles + "</b>"
                    textFormat: TextFormat.Html
                }
                Label {
                    text: "Used Cache Size: <b>" + Global.getSizeKb(app.player.cache.currentSize) + "</b>"
                    textFormat: TextFormat.Html
                }
                
                Label {
                    text: qsTr("Maximum Songs: ") + Math.round(maxCacheSongs.value)
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: groupSettingIndent
                    Slider {
                        id: maxCacheSongs
                        fromValue: 5
                        toValue: 500
                        value: app.cache.maxFiles
                    }
                }

                Label {
                    text: qsTr("Maximum Cache Size: ") + Global.getSizeKb(Math.round(maxCacheSize.value)) 
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: groupSettingIndent
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
                        app.cache.purge();
                    }
                }
                
                Divider {
                    topMargin: 30
                    bottomMargin: topMargin
                }
                
                Label {
                    text: qsTr("Personal")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    textStyle.fontWeight: FontWeight.Bold
                }
                
                Button {
                    text: qsTr("Reset Play Counts")
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 40
                    bottomMargin: 40
                    onClicked: {
                        app.catalog.resetPlayCounts();
                    }
                }

                Button {
                    text: qsTr("Reset My Favourites")
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 40
                    bottomMargin: 40
                    onClicked: {
                        app.catalog.resetMyFavourites();
                    }
                }

                Divider {
                    topMargin: 30
                    bottomMargin: topMargin
                }

                Label {
                    text: qsTr("Mixer Configuration")
                    horizontalAlignment: HorizontalAlignment.Center
                    textStyle.base: SystemDefaults.TextStyles.TitleText
                    textStyle.fontWeight: FontWeight.Bold
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
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: groupSettingIndent
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

                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: groupSettingIndent

                    Label {
                        text: qsTr("Reverb Depth")
                        visible: reverbEnabled.checked
                        textStyle.fontStyle: FontStyle.Italic
                        textStyle.fontWeight: FontWeight.W100
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

                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: groupSettingIndent
                    Label {
                        text: qsTr("Bass Amount")
                        visible: megabassEnabled.checked
                        textStyle.fontStyle: FontStyle.Italic
                        textStyle.fontWeight: FontWeight.W100
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
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: groupSettingIndent
                    Label {
                        text: qsTr("Surrond Depth")
                        visible: surroundEnabled.checked
                        textStyle.fontStyle: FontStyle.Italic
                        textStyle.fontWeight: FontWeight.W100
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