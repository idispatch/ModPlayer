import bb.cascades 1.0
import player 1.0

ActionItem {
    id: settingsActionItem
    title: qsTr("Settings") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_settings.png"
    onTriggered : {
        settingsView.createObject().open()
    }
    attachedObjects: [
        ComponentDefinition {
            id: settingsView
            source: "Settings.qml"
        }
    ]
}