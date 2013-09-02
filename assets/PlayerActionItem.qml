import bb.cascades 1.0

ActionItem {
    property variant navigationPane
    
    title: qsTr("Now Playing")
    imageSource: "asset:///images/actions/icon_nowplaying.png"
    ActionBar.placement: ActionBarPlacement.OnBar
    shortcuts: Shortcut {
        key: "p"
    } 
    onTriggered: {
        app.analytics.nowPlaying()
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