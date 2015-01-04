import bb.cascades 1.0
import player 1.0

DeleteActionItem {
    title: qsTr("Review in AppWorld") + Retranslate.onLanguageChanged
    imageSource: "asset:///images/actions/icon_review.png"
    onTriggered: {
        app.appWorld()
    }
}
