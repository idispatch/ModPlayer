import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

AlbumArtView {
    id: albumArtView
    property variant song

    fileName: song ? song.fileName : ""
    visible: song && song.fileName.length > 0 && !song.isTrackerSong && !song.isHttpSong

    scalingMethod: ScalingMethod.Fill
    loadEffect: ImageViewLoadEffect.None

    horizontalAlignment: HorizontalAlignment.Center

    topMargin: 20
    bottomMargin: 20

    minHeight: 400
    maxHeight: 450

    minWidth: 400
    maxWidth: 450

    scaleX: 0.0
    scaleY: 0.0

    onAlbumArtLoading: {
        loadingAnimation.stop()
        unloadingAnimation.play()
    }
    onAlbumArtLoaded: {
        unloadingAnimation.stop()
        loadingAnimation.play()
    }
    gestureHandlers: [
        TapHandler {
            onTapped: {
                albumArtDialog.open()
            }
        }
    ]
    attachedObjects: [
        Dialog {
            id: albumArtDialog
            onOpened: {
                displayAnimation.play()
            }
            Container {
                layout: DockLayout {}
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                background: Color.create("#C0000000")
                bottomPadding: 80
                ImageView {
                    id: largeAlbumArt
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Center
                    scalingMethod: ScalingMethod.AspectFit
                    image: albumArtView.image
                    opacity: 0.0
                    scaleX: 0.0
                    scaleY: 0.0
                    animations: [
                        ParallelAnimation {
                            id: displayAnimation
                            ScaleTransition {
                                duration: 300
                                fromX: 0.0
                                toX: 1.0
                                fromY: 0.0
                                toY: 1.0
                                easingCurve: StockCurve.CubicIn
                            }
                            FadeTransition {
                                fromOpacity: 0.0
                                toOpacity: 1.0
                                duration: 300
                                easingCurve: StockCurve.CubicIn
                            }
                        }
                    ]
                }
                Button {
                    horizontalAlignment: HorizontalAlignment.Center
                    verticalAlignment: VerticalAlignment.Bottom
                    text: qsTr("Close") + Retranslate.onLanguageChanged
                    bottomMargin: 80
                    onClicked: {
                        albumArtDialog.close()
                    }
                }
            }
        }
    ]
    animations: [
        ScaleTransition {
            id: loadingAnimation
            toX: 1.0
            toY: 1.0
            duration: 350
            easingCurve: StockCurve.BackIn
        },
        ScaleTransition {
            id: unloadingAnimation
            toX: 0.0
            toY: 0.0
            duration: 350
            easingCurve: StockCurve.BackOut
        }
    ]
}