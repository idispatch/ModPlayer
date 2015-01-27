import bb.cascades 1.0
import player 1.0

ActionItem {
    title: qsTr("Sleep Timer") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_recent.png"
    onTriggered: {
        sleepTimerSettings.createObject().open()
    }
    attachedObjects: [
        ComponentDefinition {
            id: sleepTimerSettings
            source: "SleepTimerSettings.qml"
        }
    ]
}