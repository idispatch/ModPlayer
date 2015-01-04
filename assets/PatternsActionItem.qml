import bb.cascades 1.0
import player 1.0

ActionItem {
    property variant navigationPane
    title: qsTr("Patterns") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_patterns.png"
    enabled: app.player.currentSong.songLoaded
    onTriggered : {
        var view = patternsViewDefinition.createObject()
        navigationPane.push(view)
    }
    attachedObjects: [
        ComponentDefinition {
            id: patternsViewDefinition
            source: "Patterns.qml"
        }
    ]
}