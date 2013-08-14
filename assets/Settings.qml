import bb.cascades 1.0
import "functions.js" as Global
import player 1.0

Sheet {
    id: settingsRoot
    property int groupSettingIndent: 50
    property int dividerMargin: 30
    property variant configuration: app.player.playback.getConfiguration()

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
                    text: "Cached songs: <b>" + app.player.cache.currentFiles + "</b>"
                    textFormat: TextFormat.Html
                }
                Label {
                    text: "Used cache size: <b>" + Global.getSizeKb(app.player.cache.currentSize) + "</b>"
                    textFormat: TextFormat.Html
                }

                Button {
                    text: "Purge cache"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 40
                    bottomMargin: 40
                    onClicked: {
                        app.cache.purge();
                    }
                }

                Button {
                    text: "Reset Play counts"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 40
                    bottomMargin: 40
                    onClicked: {
                        app.catalog.resetPlayCounts();
                    }
                }

                Button {
                    text: "Reset My Favourites"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 40
                    bottomMargin: 40
                    onClicked: {
                        app.catalog.resetMyFavourites();
                    }
                }

                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }

                DropDown {
                    id: output
                    title: "Output"
                    Option {
                        text: "Stereo"
                        value: 1
                        selected: settingsRoot.configuration.stereo == true 
                    }
                    Option {
                        text: "Mono"
                        value: 0
                        selected: settingsRoot.configuration.stereo == false 
                    }
                    
                    onSelectedValueChanged: {
                        settingsRoot.configuration.stereo = (selectedValue == 1)
                    }
                }

                DropDown {
                    id: bitsPerSample
                    title: "Bits per Sample"
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
                    title: "Frequency"
                    Option {
                        text: "44100 Hz"
                        description: "44100 Hz sampling rate"
                        value: 44100
                        selected: settingsRoot.configuration.frequency == value
                    }
                    Option {
                        text: "22050 Hz"
                        description: "22050 Hz sampling rate"
                        value: 22050
                        selected: settingsRoot.configuration.frequency == value
                    }
                    Option {
                        text: "11025 Hz"
                        description: "11025 Hz sampling rate"
                        value: 11025
                        selected: settingsRoot.configuration.frequency == value
                    }
                    onSelectedValueChanged: {
                        settingsRoot.configuration.frequency = selectedValue
                    }
                }

                DropDown {
                    id: resampling
                    title: "Resampling"
                    Option {
                        text: "Nearest"
                        description: "No interpolation (very fast)"
                        value: 0
                        selected: settingsRoot.configuration.resamplingMode == value
                    }
                    Option {
                        text: "Liner"
                        description: "Linear interpolation (fast, good quality)"
                        value: 1
                        selected: settingsRoot.configuration.resamplingMode == value
                    }
                    Option {
                        text: "Spline"
                        description: "Cubic spline interpolation (high quality)"
                        value: 2
                        selected: settingsRoot.configuration.resamplingMode == value
                    }
                    Option {
                        text: "FIR"
                        description: "8-tap fir filter (extremely high quality)"
                        value: 3
                        selected: settingsRoot.configuration.resamplingMode == value
                    }
                    onSelectedValueChanged: {
                        settingsRoot.configuration.resamplingMode = selectedValue
                    }
                }

                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }

                Label {
                    text: "Stereo separation"
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

                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }
                
                Label {
                    text: "Maximum number of mixing channels"
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: groupSettingIndent
                    Slider {
                        id: maximumMixingChannels
                        fromValue: 32
                        toValue: 256
                        value: settingsRoot.configuration.maximumMixingChannels
                        onValueChanged: {
                            settingsRoot.configuration.maximumMixingChannels = maximumMixingChannels.value
                        }
                    }
                }

                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }

                CheckBox {
                    id: reverbEnabled
                    text: "Enable reverb"
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
                        text: "Reverb depth"
                        enabled: reverbEnabled.checked
                    }
                    Label {
                        text: "Reverb level (quiet - loud)"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: reverbEnabled.checked
                    }
                    Slider {
                        id: reverbLevel
                        enabled: reverbEnabled.checked
                        fromValue: 0
                        toValue: 100
                        value: settingsRoot.configuration.reverbLevel
                        onValueChanged: {
                            settingsRoot.configuration.reverbLevel = reverbLevel.value
                        }
                    }
                    Label {
                        text: "Reverb delay"
                        enabled: reverbEnabled.checked
                    }
                    Label {
                        text: "Reverb delay amount"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: reverbEnabled.checked
                    }
                    Slider {
                        id: reverbDelay
                        enabled: reverbEnabled.checked
                        fromValue: 40
                        toValue: 200
                        value: settingsRoot.configuration.reverbDelay
                        onValueChanged: {
                            settingsRoot.configuration.reverbDelay = reverbDelay.value
                        }
                    }
                }

                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }

                CheckBox {
                    id: megabassEnabled
                    text: "Enable megabass"
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
                        text: "Bass amount"
                        enabled: megabassEnabled.checked
                    }
                    Label {
                        text: "XBass level (quiet - loud)"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: megabassEnabled.checked
                    }
                    Slider {
                        id: megabassLevel
                        enabled: megabassEnabled.checked
                        fromValue: 0
                        toValue: 100
                        value: settingsRoot.configuration.bassLevel
                        onValueChanged: {
                            settingsRoot.configuration.bassLevel = megabassLevel.value
                        }
                    }
                    Label {
                        enabled: megabassEnabled.checked
                        text: "Bass cutoff"
                    }
                    Label {
                        text: "XBass cutoff amount"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: megabassEnabled.checked
                    }
                    Slider {
                        id: megabassCutoff
                        enabled: megabassEnabled.checked
                        fromValue: 10
                        toValue: 100
                        value: settingsRoot.configuration.bassCutOff
                        onValueChanged: {
                            settingsRoot.configuration.bassCutOff = megabassCutoff.value
                        }
                    }
                }

                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }

                CheckBox {
                    id: surroundEnabled
                    text: "Enable surround sound"
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
                        text: "Surrond depth"
                        enabled: surroundEnabled.checked
                    }
                    Label {
                        text: "Surround level (quiet - heavy)"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: surroundEnabled.checked
                    }
                    Slider {
                        id: surroundLevel
                        enabled: surroundEnabled.checked
                        fromValue: 0
                        toValue: 100
                        value: settingsRoot.configuration.surroundLevel
                        onValueChanged: {
                            settingsRoot.configuration.surroundLevel = surroundLevel.value
                        }
                    }
                    Label {
                        text: "Surrond delay"
                        enabled: surroundEnabled.checked
                    }
                    Label {
                        text: "Surround delay amount"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: surroundEnabled.checked
                    }
                    Slider {
                        id: surroundDelay
                        enabled: surroundEnabled.checked
                        fromValue: 5
                        toValue: 50
                        value: settingsRoot.configuration.surroundDelay
                        onValueChanged: {
                            settingsRoot.configuration.surroundDelay = surroundDelay.value
                        }
                    }
                }

                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }

                CheckBox {
                    id: oversampling
                    text: "Enable oversampling"
                    checked: settingsRoot.configuration.oversamplingEnabled
                    onCheckedChanged: {
                        settingsRoot.configuration.oversamplingEnabled = oversampling.checked
                    }
                }

                CheckBox {
                    id: noiseReduction
                    text: "Enable noise reduction"
                    checked: settingsRoot.configuration.noiseReductionEnabled
                    onCheckedChanged: {
                        settingsRoot.configuration.noiseReductionEnabled = noiseReduction.checked
                    }
                }

                attachedObjects: [
                    TextStyleDefinition {
                        id: smallTextStyle
                        base: SystemDefaults.TextStyles.SmallText 
                        fontSize: FontSize.Small
                        fontStyle: FontStyle.Italic
                        color: Color.Gray
                    }
                ]
            }
        }
    }
}