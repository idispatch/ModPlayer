import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: songView

    property variant song
    property variant navigationPane 

    objectName: "SongView"
    Container {
        background: back.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill
                imageSource: "asset:///images/backgrounds/background.png"
            }
        ]
        ScrollView {
            VerticalContainer {

                leftPadding: 20
                rightPadding: 20
                topPadding: 20
                bottomPadding: 20

                visible: song != null

                HorizontalContainer {
                    SongIconView {
                        song: songView.song != null ? songView.song : null
                    }

                    VerticalContainer {
                        Label {
                            text: song != null ? "File: <b>" + song.fileName + "</b>" : ""
                            textFormat: TextFormat.Html
                        }
                        Label {
                            text: song != null ? "Size: <b>" + Global.getSizeKb(song.fileSize) + "</b>" : ""
                            textFormat: TextFormat.Html
                        }
                        Label {
                            text: song != null ? "Length: <b>" + song.songLengthText + "</b>" : ""
                            textFormat: TextFormat.Html
                        }
                        Label {
                            text: song != null ? "Title: <b>" + Global.escapeHtml(song.title) + "</b>" : ""
                            textFormat: TextFormat.Html
                        }
                    }
                }

                SongExtendedData {
                    song: songView.song != null ? songView.song : null
                }

                Divider{}

                HorizontalContainer {
                    visible: song!=null
                    ImageView {
                        imageSource: "asset:///images/badges/badge_played.png"
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        textFormat: TextFormat.Html
                        text: {
                            if(song!=null) {
                                if(song.playCount > 0) {
                                    if(song.playCount == 1) {
                                        return qsTr("You played this song once")
                                    }
                                    return "You played this song <b>" + song.playCount + "</b> times"
                                }
                                return qsTr("You did not play this song yet")
                            }
                            return "";
                        }
                    }
                    function songPlayCountChanged(){
                        if(song.id == app.player.currentSong.id) {
                            song.playCount = app.player.currentSong.playCount
                        }
                    }
                    onCreationCompleted: {
                        app.player.currentSong.playCountChanged.connect(songPlayCountChanged)
                    }
                }

                HorizontalContainer {
                    visible: song!=null && song.lastPlayed > 0
                    ImageView {
                        imageSource: "asset:///images/badges/badge_lastplayed.png"
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        text: song!=null && song.lastPlayed > 0 ? 
                                "Last played " + Global.formatTimeStamp(song.lastPlayed) : 
                                 ""
                    }
                    function songLastPlayedChanged(){
                        if(song.id == app.player.currentSong.id) {
                            song.lastPlayed = app.player.currentSong.lastPlayed
                        }
                    }
                    onCreationCompleted: {
                        app.player.currentSong.lastPlayedChanged.connect(songLastPlayedChanged)
                    }
                }

                HorizontalContainer {
                    visible: song!=null && song.myFavourite > 0
                    ImageView {
                        imageSource: "asset:///images/badges/badge_myfavourite.png"
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        text: song != null && song.myFavourite > 0 ? qsTr("You liked this song") : ""
                    }
                    function songFavouriteChanged(){
                        if(song.id == app.player.currentSong.id) {
                            song.myFavourite = app.player.currentSong.myFavourite
                        }
                    }
                    onCreationCompleted: {
                        app.player.currentSong.myFavouriteChanged.connect(songFavouriteChanged)
                    }
                }

                Label {
                    visible: song!=null && app.cache.exists(song.fileName)
                    textFormat: TextFormat.Html
                    horizontalAlignment: HorizontalAlignment.Center
                    text: getLabelText()
                    function getLabelText() {
                        if(song != null) {
                            if(app.cache.exists(song.fileName)) {
                                return "<i>" + qsTr("You have this song in the cache already") + "</i>"
                            }
                        }
                        return "";
                    }
                    function currentCacheFilesChanged() {
                        visible = song!=null && app.cache.exists(song.fileName)
                        text = getLabelText()
                    }
                    onCreationCompleted: {
                        app.cache.currentFilesChanged.connect(currentCacheFilesChanged)
                    }
                }

                Divider{}

                SongOtherStats {
                    song: songView != null ? songView.song : null
                }

                Divider{}

                SongStatData {
                    song: songView != null ? songView.song : null
                }
            }
        }
    }

    function play() {
        if(song!=null) {
            showPlayer()
            app.player.play(song)
        }
    }

    function showPlayer() {
        var view = songPlayer.createObject()
        navigationPane.push(view)
    }

    function load(songId) {
        song = app.player.catalog.resolveModuleById(songId)
    }

    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]

    actions: [ 
        ActionItem {
            title: qsTr("Now Playing")
            imageSource: "asset:///images/actions/icon_nowplaying.png"
            ActionBar.placement: ActionBarPlacement.InOverflow
            shortcuts: Shortcut {
                key: "n"
            } 
            onTriggered: {
                showPlayer()
            }
        },
        ActionItem {
            title: "Play"
            imageSource: "asset:///images/actions/icon_play.png"
            enabled: song != null
            ActionBar.placement: ActionBarPlacement.OnBar
            shortcuts: Shortcut {
                key: "p"
            } 
            onTriggered: {
                play()
            }
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AddFavouriteActionItem {
            currentSong: song
        },
        RemoveFavouriteActionItem {
            currentSong: song
        }
    ]
}
