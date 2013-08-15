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
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                
                leftPadding: 20
                rightPadding: 20
                topPadding: 20
                bottomPadding: 20
                
                visible: song != null
                
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    SongIconView {
                        song: songView.song != null ? songView.song : null
                    }
                    Container {
                        layout: StackLayout {
                            orientation: LayoutOrientation.TopToBottom
                        }
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

                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
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
                }
                Container {
                    layout: StackLayout {
                        orientation: LayoutOrientation.LeftToRight
                    }
                    visible: song!=null && song.playCount > 0
                    ImageView {
                        imageSource: "asset:///images/badges/badge_lastplayed.png"
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        text: {
                            if(song!=null) {
                                if(song.playCount > 0) {
                                    return "Last played " + Global.formatTimeStamp(song.lastPlayed)
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
                    visible: song!=null && song.myFavourite > 0
                    ImageView {
                        imageSource: "asset:///images/badges/badge_myfavourite.png"
                        verticalAlignment: VerticalAlignment.Center
                        rightMargin: 5
                    }
                    Label {
                        text: {
                            if(song != null) {
                                if(song.myFavourite > 0) {
                                    return qsTr("You liked this song")
                                }
                            }
                            return ""
                        }
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
            var view = songPlayer.createObject()
            view.navigationPane = navigationPane 
            navigationPane.push(view)
            app.player.play(song)
            song.playCount = song.playCount + 1
            song.lastPlayed = (new Date()).getTime() / 1000 
        }
    }
    
    function showPlayer() {
        var view = songPlayer.createObject()
        view.navigationPane = navigationPane 
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
            currentSong: parent.song
        },
        RemoveFavouriteActionItem {
            currentSong: parent.song
        }
    ]
}
