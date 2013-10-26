import bb.cascades 1.0
import bb.system 1.0
import bb.cascades.pickers 1.0
import player 1.0

import "functions.js" as Global

ActionItem {
    property string currentSong
    property bool certain: true
    title: qsTr("Save Song to Device")
    imageSource: "asset:///images/actions/icon_save_song.png"
    ActionBar.placement: ActionBarPlacement.InOverflow
    enabled: certain && app.cache.exists(currentSong)
    onTriggered: {
        var fileName = Global.fileNameOnly(currentSong)

        console.log(title + ", currentSong: " + currentSong)
        console.log(title + ", fileName: " + fileName)

        filePicker.defaultSaveFileNames = [fileName]
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
            type : FileType.Other
            mode: FilePickerMode.Saver
            allowOverwrite: true
            title : qsTr("Save Song")
            directories : ["/accounts/1000/shared/downloads"]
            onFileSelected : {
                if(selectedFiles.length > 0) 
                {
                    var newFileName = selectedFiles[0]
                    
                    //app.cache.save(cacheFileName, newFileName)
                    app.player.exportMp3(cacheFileName, newFileName + ".mp3")
                    
                    app.player.userDirectory = Global.directoryOnly(newFileName)
                    fileSaved.body = qsTr("The song %1 has been saved").arg(Global.fileNameOnly(newFileName)) 
                    fileSaved.show()
                }
            }
        },
        SystemToast {
            id: fileSaved
        }
    ]
}