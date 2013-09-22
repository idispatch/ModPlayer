import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant navigationPane
    title: qsTr("Load song from device")
    //imageSource: "asset:///images/actions/icon_pause.png"
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        app.player.load()
    }
    onCreationCompleted: {
        app.player.localSongLoaded.connect(onLocalSongLoaded)
    }
    function onLocalSongLoaded() {
        var view = songPlayer.createObject()
        view.navigationPane = navigationPane
        navigationPane.push(view)
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
}