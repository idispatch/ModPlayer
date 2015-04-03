import bb.cascades 1.3
import bb.system 1.2
import player 1.0

ActionItem {
    title: qsTr("Equalizer Preset") + Retranslate.onLanguageChanged
    imageSource: app.isExtendedVersion ? "asset:///images/actions/icon_equalizer.png" : "asset:///images/actions/icon_lock.png"
    enabled: !app.player.currentSong.isTrackerSong
    onTriggered: selectEqualizerMode.run()
    attachedObjects: [
        SystemListDialog {
            id: selectEqualizerMode
            title: qsTr("Equalizer Preset") + Retranslate.onLanguageChanged
            selectionMode: ListSelectionMode.Single
            modality: SystemUiModality.Application
            includeRememberMe: false
            emoticonsEnabled: false
            defaultButton: SystemUiButton.confirmButton
            dismissAutomatically: true
            function run() {
                if(app.isExtendedVersion) {
                    var mode = app.player.equalizerPreset
                    clearList()
                    appendItem("Default", true, mode == 0)
                    appendItem("Airplane", true, mode == 1)
                    appendItem("Bass Boost", true, mode == 2)
                    appendItem("Treble Boost", true, mode == 3)
                    appendItem("Voice Boost", true, mode == 4)
                    appendItem("Bass Lower", true, mode == 5)
                    appendItem("Treble Lower", true, mode == 6)
                    appendItem("Voice Lower", true, mode == 7)
                    appendItem("Acoustic", true, mode == 8)
                    appendItem("Dance", true, mode == 9)
                    appendItem("Electronic", true, mode == 10)
                    appendItem("HipHop", true, mode == 11)
                    appendItem("Jazz", true, mode == 12)
                    appendItem("Lounge", true, mode == 13)
                    appendItem("Piano", true, mode == 14)
                    appendItem("Rhythm And Blues", true, mode == 15)
                    appendItem("Rock", true, mode == 16)
                    appendItem("Spoken Word", true, mode == 17)
                    exec()
                    if(result == SystemUiResult.ConfirmButtonSelection ) {
                        mode = selectedIndices[0]
                    }
                    app.player.equalizerPreset = mode
                } else {
                    app.pleaseBuy()
                }
            }
        }
    ]
}