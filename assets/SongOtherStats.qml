import bb.cascades 1.0

VerticalContainer {
    property variant song

    background: backgroundImagePaint.imagePaint

    topPadding: 12
    bottomPadding: 12
    leftPadding: 16
    rightPadding: 16

    horizontalAlignment: HorizontalAlignment.Fill

    topMargin: 16
    bottomMargin: 16

    HorizontalContainer {
        visible: song!=null
        ImageView {
            imageSource: "asset:///images/badges/badge_downloads.png"
            visible: song != null && song.downloads > 0
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            textFormat: TextFormat.Html
            text: {
                if(song != null) {
                    if(song.downloads > 0) {
                        return "Dowloaded <b>" + song.downloads + "</b> times by others"
                    }
                }
                return "";
            }
        }
    }
    HorizontalContainer {
        visible: song!=null
        ImageView {
            imageSource: "asset:///images/badges/badge_favourite.png"
            visible: song !=null && song.favourited > 0
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            textFormat: TextFormat.Html
            text: {
                if(song != null) {
                    if(song.favourited > 0) {
                        return "Favourited by <b>" + song.favourited + "</b> people"
                    }
                    return qsTr("Not favourited by anyone yet")
                }
                return "";
            }
        }
    }
    HorizontalContainer {
        visible: song!=null
        ImageView {
            imageSource: "asset:///images/badges/badge_score.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
            visible: song != null && song.score > 0
        }
        Label {
            textFormat: TextFormat.Html
            text: {
                if(song != null) {
                    if(song.score > 0) {
                        return "Rated <b>" + song.score + "</b> of <b>10</b> by others"
                    }
                    return qsTr("Not rated by anyone yet")
                }
                return "";
            }
        }
    }
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundImagePaint
            repeatPattern: RepeatPattern.Fill
            imageSource: "asset:///images/backgrounds/container_back.amd"
        }
    ]
}