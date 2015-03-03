import bb.cascades 1.3
import player 1.0

ActionItem {
    property variant navigationPane
    
    title: qsTr("Now Playing") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_nowplaying.png"
    shortcuts: Shortcut {
        key: "n"
    } 
    onTriggered: {
        app.analytics.nowPlaying()
        var view = songPlayer.createObject()
        if(view) {
            view.navigationPane = navigationPane
            navigationPane.push(view)
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
}