import bb.cascades 1.0

GroupContainer {
    property variant song
    visible: song != null && !song.isLocal
    onCreationCompleted: {
        publicInfoAnimation.play()
    }
    animations: [
        ScaleTransition {
            id: publicInfoAnimation
            fromY: 0.0
            toY: 1.0
            duration: 400
            easingCurve: StockCurve.CubicInOut
        }
    ]
    VerticalContainer {
        horizontalAlignment: HorizontalAlignment.Fill
        visible: song != null && !song.isLocal
        HorizontalContainer {
            visible: song != null && song.downloads > 0 && !song.isLocal
            ImageView {
                imageSource: "asset:///images/badges/badge_downloads.png"
                visible: song && song.downloads > 0 && !song.isLocal
                verticalAlignment: VerticalAlignment.Center
                rightMargin: 5
            }
            BlackLabel {
                textFormat: TextFormat.Html
                text: {
                    if(song) {
                        if(song.downloads > 0) {
                            return qsTr("Dowloaded <b>%1</b> times by others").arg(song.downloads)
                        }
                    }
                    return ""
                }
            }
        }
        HorizontalContainer {
            visible: song != null && !song.isLocal
            ImageView {
                imageSource: "asset:///images/badges/badge_favourite.png"
                visible: song && song.favourited > 0 && !song.isLocal
                verticalAlignment: VerticalAlignment.Center
                rightMargin: 5
            }
            BlackLabel {
                textFormat: TextFormat.Html
                text: {
                    if(song) {
                        if(song.favourited > 0) {
                            return qsTr("Favourited by <b>%1</b> people").arg(song.favourited)
                        }
                        return qsTr("Not favourited by anyone yet")
                    }
                    return ""
                }
            }
        }
        HorizontalContainer {
            visible: song != null && !song.isLocal
            ImageView {
                imageSource: "asset:///images/badges/badge_score.png"
                verticalAlignment: VerticalAlignment.Center
                rightMargin: 5
                visible: song && song.score > 0 && !song.isLocal
            }
            BlackLabel {
                textFormat: TextFormat.Html
                text: {
                    if(song) {
                        if(song.score > 0) {
                            return qsTr("Rated <b>%1</b> of <b>10</b> by others").arg(song.score)
                        }
                        return qsTr("Not rated by anyone yet")
                    }
                    return ""
                }
            }
        }
    }
}
