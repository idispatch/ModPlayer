import bb.cascades 1.0

ActionItem {
    property variant navigationPane
    
    title: "Player"
    imageSource: "asset:///images/icon_player.png"
    ActionBar.placement: ActionBarPlacement.OnBar
    shortcuts: Shortcut {
        key: "p"
    } 
    onTriggered: {
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