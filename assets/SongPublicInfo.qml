import bb.cascades 1.0

GroupContainer {
    property variant song
    VerticalContainer {
        horizontalAlignment: HorizontalAlignment.Fill
        HorizontalContainer {
            visible: song != null && song.downloads > 0
            ImageView {
                imageSource: "asset:///images/badges/badge_downloads.png"
                visible: song && song.downloads > 0
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
                    return "";
                }
            }
        }
        HorizontalContainer {
            visible: song != null
            ImageView {
                imageSource: "asset:///images/badges/badge_favourite.png"
                visible: song && song.favourited > 0
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
                    return "";
                }
            }
        }
        HorizontalContainer {
            visible: song != null
            ImageView {
                imageSource: "asset:///images/badges/badge_score.png"
                verticalAlignment: VerticalAlignment.Center
                rightMargin: 5
                visible: song && song.score > 0
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
                    return "";
                }
            }
        }
    }
}
