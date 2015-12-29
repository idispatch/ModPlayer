import bb.cascades 1.3
import player 1.0

ActionItem {
    property variant navigationPane
    title: qsTr("Instruments and Samples") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_samples.png"
    enabled: app.player.currentSong.songLoaded && app.player.currentSong.isTrackerSong
    onTriggered : {
        var view = instrumentsViewDefinition.createObject()
        if(view) {
            view.navigationPane = navigationPane
            navigationPane.push(view)
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: instrumentsViewDefinition
            source: "Instruments.qml"
        }
    ]
}