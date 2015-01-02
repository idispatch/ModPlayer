import bb.cascades 1.0
import bb.system 1.0
import bb.platform 1.0
import player 1.0

ActionItem {
    title: qsTr("Buy ModPlayer Plus") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_buy.png"
    enabled: !app.isExtendedVersion
    onTriggered: buyView.createObject().open()
    attachedObjects: [
        ComponentDefinition {
            id: buyView
            source: "Buy.qml"
        }
    ]
}