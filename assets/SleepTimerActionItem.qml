import bb.cascades 1.0
import player 1.0

ActionItem {
    title: qsTr("Sleep Timer") + Retranslate.onLanguageChanged
    imageSource: app.isExtendedVersion ? "asset:///images/actions/icon_recent.png" : "asset:///images/actions/icon_lock.png"
    onTriggered: {
        if(app.isExtendedVersion) {
            sleepTimerSettings.createObject().open()
        } else {
            pleaseBuy.exec()
        }
    }
    attachedObjects: [
        ComponentDefinition {
            id: sleepTimerSettings
            source: "SleepTimerSettings.qml"
        },
        PleaseBuy {
            id: pleaseBuy
        }
    ]
}