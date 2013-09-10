import bb.cascades 1.0

ActionItem {
    property variant navigationPane
    title: qsTr("Instruments and Samples")
    //imageSource: "asset:///images/actions/icon_like.png"
    enabled: app.player.currentSong.songLoaded
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered : {
        var view = instrumentsView.createObject()
        navigationPane.push(view)
    }
    attachedObjects: [
        ComponentDefinition {
            id: instrumentsView
            source: "Instruments.qml"
        }
    ]
}