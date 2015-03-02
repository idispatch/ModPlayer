import bb.cascades 1.0
import bb.system 1.0
import bb.cascades.pickers 1.0
import player 1.0

import "functions.js" as Global

ActionItem {
    id: saveSongActionItem
    property string currentSong
    property bool certain: true
    title: qsTr("Save Song to Device") + Retranslate.onLanguageChanged
    imageSource: app.isExtendedVersion ? "asset:///images/actions/icon_save_song.png" : "asset:///images/actions/icon_lock.png"
    enabled: certain && app.cache.fileExists(currentSong)
    onTriggered: {
        if(!app.isExtendedVersion) {
            app.pleaseBuy()
            return
        }
        var result = selectSaveFormat.run()
        if(result.length < 1)
            return
        var saveFileName 
        if(result == 'mp3') {
            saveFileName = Global.fileNameOnly(currentSong)
            saveFileName = Global.replaceExtension(saveFileName, ".mp3")
        } else {
            saveFileName = Global.fileNameOnly(currentSong)
        }
        filePicker.saveFormat = result
        filePicker.defaultSaveFileNames = [saveFileName]
        if(Global.isAbsolutePath(currentSong)) {
            filePicker.cacheFileName = currentSong
        } else {
            filePicker.cacheFileName = Global.pathJoin(app.cache.cachePath, currentSong)
        } 
        if(app.player.userDirectory.length > 0) {
            filePicker.directories = [app.player.userDirectory]
        }
        filePicker.open()
    }
    onCreationCompleted: {
        app.cache.currentFilesChanged.connect(function() {
            // a trick to refresh enabled: property
            certain = false
            certain = true
        })
    }
    attachedObjects: [
        FilePicker {
            id: filePicker
            property string cacheFileName
            property string saveFormat
            type : FileType.Other
            mode: FilePickerMode.Saver
            allowOverwrite: true
            title : qsTr("Save Song") + Retranslate.onLanguageChanged
            directories : ["/accounts/1000/shared/downloads"]
            onFileSelected : {
                if(selectedFiles.length != 1)
                    return 
                var newFileName = selectedFiles[0]
                if(saveFormat == 'mp3') {
                    app.player.exportMp3(cacheFileName, newFileName)
                } else {
                    app.cache.save(cacheFileName, newFileName)
                }
                app.player.userDirectory = Global.directoryOnly(newFileName)
                fileSaved.body = qsTr("The song %1 has been saved").arg(Global.fileNameOnly(newFileName)) + Retranslate.onLanguageChanged 
                fileSaved.show()
            }
        },
        SystemToast {
            id: fileSaved
        },
        SystemListDialog {
            id: selectSaveFormat
            title: qsTr("Select Save Format") + Retranslate.onLanguageChanged
            selectionMode: ListSelectionMode.Single
            modality: SystemUiModality.Application
            includeRememberMe: false
            emoticonsEnabled: false
            defaultButton: SystemUiButton.confirmButton
            dismissAutomatically: true
            function run() {
                var selectedOption = ''
                clearList()
                appendItem(qsTr("Original format") + Retranslate.onLanguageChanged, true, true)
                appendItem(qsTr("MP3 format") + Retranslate.onLanguageChanged, true, false)
                exec()
                if(result == SystemUiResult.ConfirmButtonSelection ){
                    if(selectedIndices.length == 1) {
                        if(selectedIndices[0] == 0) {
                            selectedOption = 'original'
                        } else {
                            selectedOption = 'mp3'
                        }
                    }
                }
                return selectedOption
            }
        }
    ]
}