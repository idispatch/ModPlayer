import bb.cascades 1.0

Container {
    layout: DockLayout {}
    property alias running: activityIndicator.running
    visible: running
    horizontalAlignment: HorizontalAlignment.Fill
    verticalAlignment: VerticalAlignment.Fill
    topPadding: 200
    GroupContainer {
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Center
        ActivityIndicator {
            id: activityIndicator
            running: false
            preferredHeight: 128
            preferredWidth: preferredHeight
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
        }
    }
}
