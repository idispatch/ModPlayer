import bb.cascades 1.0
import player 1.0

ActionItem {
    title: qsTr("Sleep Timer") + Retranslate.onLanguageChanged
    imageSource: app.isExtendedVersion ? "asset:///images/actions/icon_recent.png" : "asset:///images/actions/icon_lock.png"
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