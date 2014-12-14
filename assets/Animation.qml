import bb.cascades 1.0

HorizontalContainer {
    horizontalAlignment: HorizontalAlignment.Center
    background: Color.Black
    leftPadding: 4
    rightPadding: 4
    topPadding: 4
    bottomPadding: 4
    ImageView {
        id: animationView
        imageSource: "asset:///images/animation/modplayer.gif"
        attachedObjects: [
            ImageAnimator {
                animatedImage: animationView.image
                started: true
            }
        ]
    }
}
