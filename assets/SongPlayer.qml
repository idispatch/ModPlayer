import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Page {
    id: songPlayer
    objectName: "songPlayer"
    property variant navigationPane
    ViewContainer {
        ScrollView {
            VerticalContainer {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                leftPadding: 20
                rightPadding: 20
                VerticalContainer {
                    visible: app.player.currentSong.songLoaded
                    horizontalAlignment: HorizontalAlignment.Fill
                    topPadding: 20
                    bottomPadding: 20
                    Container {
                        background: Color.White
                        leftPadding: 1
                        rightPadding: leftPadding
                        topPadding: 1
                        bottomPadding: topPadding
                        horizontalAlignment: HorizontalAlignment.Center
                        LCDDisplay {
                            text: {
                                if(app.player.currentSong.title.length == 0) {
                                    return Global.fileNameOnly(app.player.currentSong.fileName)
                                } else {
                                    return app.player.currentSong.title
                                }
                            }
                            horizontalAlignment: HorizontalAlignment.Center
                        }
                    }
                    SongMainInfo {
                        song: app.player.currentSong
                    }
                    SongPublicInfo {
                        song: app.player.currentSong
                        visible: app.player.currentSong.songLoaded && app.player.currentSong.id != 0
                    }
                    SongParametersInfo {
                        song: app.player.currentSong
                    }
                    SongDynamicInfo {
                        song: app.player.currentSong
                    }
                }
                Label {
                    text: app.player.statusText
                    textStyle {
                        fontWeight: FontWeight.Bold
                        fontSize: FontSize.Large
                        color: Color.White
                    }
                }
            }
        }
    }
    actions: [
        PlayActionItem {},
        PauseActionItem {}, 
        SameArtistActionItem {
            currentSong: app.player.currentSong
            navigationPane: songPlayer.navigationPane 
        },
        InstrumentsActionItem {
            navigationPane: songPlayer.navigationPane
        },
        AddFavouriteActionItem {
            currentSong: app.player.currentSong
        },
        RemoveFavouriteActionItem {
            currentSong: app.player.currentSong
        },
        SaveSongActionItem{
            currentSong: app.player.currentSong.fileName
            enabled: app.player.currentSong.songLoaded && app.player.currentSong.id != 0
        },
        OpenSongActionItem{},
        AppWorldActionItem{}
    ]
}
