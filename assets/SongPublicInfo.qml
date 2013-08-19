import bb.cascades 1.0

GroupContainer {
    property variant song
    VerticalContainer {
        horizontalAlignment: HorizontalAlignment.Fill
        HorizontalContainer {
            visible: song != null
            ImageView {
                imageSource: "asset:///images/badges/badge_downloads.png"
                visible: song && song.downloads > 0
                verticalAlignment: VerticalAlignment.Center
                rightMargin: 5
            }
            Label {
                textFormat: TextFormat.Html
                text: {
                    if(song) {
                        if(song.downloads > 0) {
                            return "Dowloaded <b>" + song.downloads + "</b> times by others"
                        }
                    }
                    return "";
                }
                textStyle.color: Color.Black
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
            Label {
                textFormat: TextFormat.Html
                text: {
                    if(song) {
                        if(song.favourited > 0) {
                            return "Favourited by <b>" + song.favourited + "</b> people"
                        }
                        return qsTr("Not favourited by anyone yet")
                    }
                    return "";
                }
                textStyle.color: Color.Black
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
            Label {
                textFormat: TextFormat.Html
                textStyle.color: Color.Black
                text: {
                    if(song) {
                        if(song.score > 0) {
                            return "Rated <b>" + song.score + "</b> of <b>10</b> by others"
                        }
                        return qsTr("Not rated by anyone yet")
                    }
                    return "";
                }
            }
        }
    }
}
