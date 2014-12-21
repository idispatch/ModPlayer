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
                id: imageAnimator
                animatedImage: animationView.image
                started: true
                onCreationCompleted: {
                    Application.fullscreen.connect(function(){
                        imageAnimator.started = true
                    })
                    Application.invisible.connect(function(){
                        imageAnimator.started = false
                    })
                    Application.asleep.connect(function(){
                        imageAnimator.started = false
                    })
                    Application.thumbnail.connect(function(){
                        imageAnimator.started = false
                    })
                }
            }
        ]
    }
}
