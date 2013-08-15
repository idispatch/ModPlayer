import bb.cascades 1.0

Container {
    id: songStatsRoot
    property variant song
     
    layout: StackLayout {
        orientation: LayoutOrientation.TopToBottom
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        visible: songStatsRoot.song!=null
        ImageView {
            imageSource: "asset:///images/badges/badge_downloads.png"
            visible: songStatsRoot.song != null && songStatsRoot.song.downloads > 0
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            textFormat: TextFormat.Html
            text: {
                if(songStatsRoot.song != null) {
                    if(songStatsRoot.song.downloads > 0) {
                        return "Dowloaded <b>" + songStatsRoot.song.downloads + "</b> times by others"
                    }
                }
                return "";
            }
        }
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        visible: songStatsRoot.song!=null
        ImageView {
            imageSource: "asset:///images/badges/badge_favourite.png"
            visible: songStatsRoot.song !=null && songStatsRoot.song.favourited > 0
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
        }
        Label {
            textFormat: TextFormat.Html
            text: {
                if(songStatsRoot.song != null) {
                    if(songStatsRoot.song.favourited > 0) {
                        return "Favourited by <b>" + songStatsRoot.song.favourited + "</b> people"
                    }
                    return qsTr("Not favourited by anyone yet")
                }
                return "";
            }
        }
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.LeftToRight
        }
        visible: songStatsRoot.song!=null
        ImageView {
            imageSource: "asset:///images/badges/badge_score.png"
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 5
            visible: songStatsRoot.song != null && songStatsRoot.song.score > 0
        }
        Label {
            textFormat: TextFormat.Html
            text: {
                if(songStatsRoot.song != null) {
                    if(songStatsRoot.song.score > 0) {
                        return "Rated <b>" + songStatsRoot.song.score + "</b> of <b>10</b> by others"
                    }
                    return qsTr("Not rated by anyone yet")
                }
                return "";
            }
        }
    }
}