import bb.cascades 1.3

ActionItem {
    property variant navigationPane
    title: qsTr("Instruments and Samples")
    imageSource: "asset:///images/actions/icon_samples.png"
    enabled: app.player.currentSong.songLoaded && app.player.currentSong.isTrackerSong
    onTriggered : {
        var view = instrumentsViewDefinition.createObject()
        navigationPane.push(view)
    }
    attachedObjects: [
        ComponentDefinition {
            id: instrumentsViewDefinition
            source: "Instruments.qml"
        }
    ]
}