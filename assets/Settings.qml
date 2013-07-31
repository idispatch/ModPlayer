import bb.cascades 1.1
import "functions.js" as Global

Sheet {
    id: sheetConfig
    
    property int groupSettingIndent: 50
    property int dividerMargin: 30
    
    Page {
        titleBar: TitleBar {
            title: qsTr("Configuration")
            visibility: ChromeVisibility.Visible
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    sheetConfig.close();
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Apply")
                onTriggered: {
                    sheetConfig.close();
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
                    topMargin: sheetConfig.dividerMargin
                    bottomMargin: sheetConfig.dividerMargin
                }
                
                Label {
                    text: "Stereo separation"
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: sheetConfig.groupSettingIndent
                    Slider {
                        fromValue: 1
                        toValue: 256
                        value: 128
                    }
                }
                
                Divider {
                    topMargin: sheetConfig.dividerMargin
                    bottomMargin: sheetConfig.dividerMargin
                }
                
                Label {
                    text: "Maximum number of mixing channels"
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: sheetConfig.groupSettingIndent
                    Slider {
                        fromValue: 32
                        toValue: 256
                        value: 128
                    }
                }
                
                Divider {
                    topMargin: sheetConfig.dividerMargin
                    bottomMargin: sheetConfig.dividerMargin
                }
                
                CheckBox {
                    id: reverb
                    text: "Enable reverb"
                    checked: true
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: sheetConfig.groupSettingIndent
                    
                    Label {
                        text: "Reverb depth"
                        enabled: reverb.checked
                    }
                    Label {
                        text: "Reverb level 0(quiet)-100(loud)"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: reverb.checked
                    }
                    Slider {
                        enabled: reverb.checked
                        fromValue: 0
                        toValue: 100
                        value: 50
                    }
                    Label {
                        text: "Reverb delay"
                        enabled: reverb.checked
                    }
                    Label {
                        text: "Reverb delay in ms, usually 40-200ms"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: reverb.checked
                    }
                    Slider {
                        enabled: reverb.checked
                        fromValue: 40
                        toValue: 200
                        value: 128
                    }
                }
                
                Divider {
                    topMargin: sheetConfig.dividerMargin
                    bottomMargin: sheetConfig.dividerMargin
                }
                
                CheckBox {
                    id: megabass
                    text: "Enable megabass"
                    checked: true
                }
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: sheetConfig.groupSettingIndent
                    Label {
                        text: "Bass amount"
                        enabled: megabass.checked
                    }
                    Label {
                        text: "XBass level 0(quiet)-100(loud)"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: megabass.checked
                    }
                    Slider {
                        enabled: megabass.checked
                        fromValue: 0
                        toValue: 100
                        value: 50
                    }
                    Label {
                        enabled: megabass.checked
                        text: "Bass cutoff"
                    }
                    Label {
                        text: "XBass cutoff in Hz 10-100"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: megabass.checked
                    }
                    Slider {
                        enabled: megabass.checked
                        fromValue: 10
                        toValue: 100
                        value: 50
                    }
                }
                
                Divider {
                    topMargin: sheetConfig.dividerMargin
                    bottomMargin: sheetConfig.dividerMargin
                }
                
                CheckBox {
                    id: surround
                    text: "Enable surround sound"
                    checked: true
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    leftPadding: sheetConfig.groupSettingIndent
                    Label {
                        text: "Surrond depth"
                        enabled: surround.checked
                    }
                    Label {
                        text: "Surround level 0(quiet) - 100(heavy)"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: surround.checked
                    }
                    Slider {
                        enabled: surround.checked
                        fromValue: 0
                        toValue: 100
                        value: 50
                    }
                    Label {
                        text: "Surrond delay"
                        enabled: surround.checked
                    }
                    Label {
                        text: "Surround delay in milliseconds, usually 5-40ms"
                        textStyle {
                            base: smallTextStyle.style
                        }
                        enabled: surround.checked
                    }
                    Slider {
                        enabled: surround.checked
                        fromValue: 5
                        toValue: 50
                        value: 20
                    }
                }
                
                Divider {
                    topMargin: sheetConfig.dividerMargin
                    bottomMargin: sheetConfig.dividerMargin
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
                    topMargin: sheetConfig.dividerMargin
                    bottomMargin: sheetConfig.dividerMargin
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