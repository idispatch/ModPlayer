import bb.cascades 1.1
import "functions.js" as Global

Sheet {
    id: seetingsRoot
    property int groupSettingIndent: 50
    property int dividerMargin: 30
    
    Page {
        titleBar: TitleBar {
            title: qsTr("Configuration")
            visibility: ChromeVisibility.Visible
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    seetingsRoot.close();
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Apply")
                onTriggered: {
                    seetingsRoot.close();
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
                
                DropDown {
                    id: output
                    title: "Output"
                    Option {
                        text: "Stereo"
                        value: 2
                        selected: true
                    }
                    Option {
                        text: "Mono"
                        value: 1
                        selected: false
                    }
                }
                DropDown {
                    id: bitsPerSample
                    title: "Bits per Sample"
                    Option {
                        text: "8"
                        value: 8
                        selected: false
                    }
                    Option {
                        text: "16"
                        value: 16
                        selected: true
                    }
                    Option {
                        text: "32"
                        value: 32
                        selected: false
                    }
                }
                DropDown {
                    id: frequency
                    title: "Frequency"
                    Option {
                        text: "44100 Hz"
                        description: "44100 Hz sampling rate"
                        value: 44100
                        selected: true
                    }
                    Option {
                        text: "22050 Hz"
                        description: "22050 Hz sampling rate"
                        value: 22050
                        selected: false
                    }
                    Option {
                        text: "11025 Hz"
                        description: "11025 Hz sampling rate"
                        value: 11025
                        selected: false
                    }
                }
                DropDown {
                    id: resampling
                    title: "Resampling"
                    Option {
                        text: "Nearest"
                        description: "No interpolation (very fast)"
                        value: 0
                        selected: true
                    }
                    Option {
                        text: "Liner"
                        description: "Linear interpolation (fast, good quality)"
                        value: 1
                        selected: false
                    }
                    Option {
                        text: "Spline"
                        description: "Cubic spline interpolation (high quality)"
                        value: 2
                        selected: false
                    }
                    Option {
                        text: "FIR"
                        description: "8-tap fir filter (extremely high quality)"
                        value: 3
                        selected: false
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
                        value: 128
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
                        value: 128
                    }
                }
                
                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }
                
                CheckBox {
                    id: reverbEnabled
                    text: "Enable reverb"
                    checked: true
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
                        text: "Reverb level 0(quiet)-100(loud)"
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
                        value: 50
                    }
                    Label {
                        text: "Reverb delay"
                        enabled: reverbEnabled.checked
                    }
                    Label {
                        text: "Reverb delay in ms, usually 40-200ms"
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
                        value: 128
                    }
                }
                
                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }
                
                CheckBox {
                    id: megabassEnabled
                    text: "Enable megabass"
                    checked: true
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
                        text: "XBass level 0(quiet)-100(loud)"
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
                        value: 50
                    }
                    Label {
                        enabled: megabassEnabled.checked
                        text: "Bass cutoff"
                    }
                    Label {
                        text: "XBass cutoff in Hz 10-100"
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
                        value: 50
                    }
                }
                
                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }
                
                CheckBox {
                    id: surroundEnabled
                    text: "Enable surround sound"
                    checked: true
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
                        text: "Surround level 0(quiet) - 100(heavy)"
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
                        value: 50
                    }
                    Label {
                        text: "Surrond delay"
                        enabled: surroundEnabled.checked
                    }
                    Label {
                        text: "Surround delay in milliseconds, usually 5-40ms"
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
                        value: 20
                    }
                }
                
                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }
                
                CheckBox {
                    id: oversampling
                    text: "Enable oversampling"
                    checked: true
                }
                
                CheckBox {
                    id: noiseReduction
                    text: "Enable noise reduction"
                    checked: true
                }
                
                Divider {
                    topMargin: dividerMargin
                    bottomMargin: dividerMargin
                }
                
                Label {
                    text: "Cached songs: " + app.player.cache.currentFiles
                    textStyle {
                        base: boldTextStyle.style
                    }
                }
                Label {
                    text: "Used cache size: " + Global.getSizeKb(app.player.cache.currentSize)
                    textStyle {
                        base: boldTextStyle.style
                    }
                }
                
                Button {
                    text:"Purge cache"
                    horizontalAlignment: HorizontalAlignment.Center
                    topMargin: 50
                    bottomMargin: 50
                    onClicked: {
                        app.cache.purge();
                    }
                }
                
                attachedObjects: [
                    TextStyleDefinition
                    {
                        id: smallTextStyle
                        base: SystemDefaults.TextStyles.SmallText 
                        fontSize: FontSize.Small
                        fontStyle: FontStyle.Italic
                        color: Color.Gray
                    },
                    TextStyleDefinition
                    {
                        id: boldTextStyle
                        base: SystemDefaults.TextStyles.BodyText
                        fontWeight: FontWeight.Bold 
                    }
                ]
            }
        }
    }
}