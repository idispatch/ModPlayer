import bb.cascades 1.0

DeleteActionItem {
    title: qsTr("Review in AppWorld")
    imageSource: "asset:///images/actions/icon_review.png"
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        app.appWorld()
    }
}
