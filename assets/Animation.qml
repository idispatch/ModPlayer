import bb.cascades 1.3
import player 1.0

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
                    var animator = imageAnimator
                    Application.fullscreen.connect(function(){
                        animator.started = true
                    })
                    Application.invisible.connect(function(){
                        animator.started = false
                    })
                    Application.asleep.connect(function(){
                        animator.started = false
                    })
                    Application.thumbnail.connect(function(){
                        animator.started = false
                    })
                }
            }
        ]
    }
}
