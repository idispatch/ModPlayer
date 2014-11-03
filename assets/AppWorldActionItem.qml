import bb.cascades 1.3

DeleteActionItem {
    title: qsTr("Review in AppWorld")
    imageSource: "asset:///images/actions/icon_review.png"
    onTriggered: {
        app.appWorld()
    }
}
