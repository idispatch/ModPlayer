import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: songView
    objectName: "songView"
    property variant song
    property variant navigationPane
    ViewContainer {
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            VerticalContainer {
                leftPadding: 20
                rightPadding: 20
                topPadding: 20
                bottomPadding: 20

                visible: song != null
                horizontalAlignment: HorizontalAlignment.Fill

                SongMainInfo {
                    song: songView.song
                }
                SongExtendedInfo {
                    song: songView.song
                }
                SongPersonalInfo {
                    song: songView.song
                }
                SongPublicInfo {
                    song: songView.song
                }
                SongParametersInfo {
                    song: songView.song
                }
            }
        }
    }
    function load(songId) {
        song = app.player.catalog.resolveModuleById(songId, songView)
        app.analytics.view(song.id, Global.fileNameOnly(song.fileName))
    }
    function showPlayerView() {
        if(mainTabPane.activePane == navigationPane && navigationPane.top == songView) {
            var view = songPlayer.createObject()
            view.navigationPane = navigationPane
            navigationPane.push(view)
        }
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(showPlayerView)
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
    actions: [ 
        ActionItem {
            title: qsTr("Play")
            imageSource: "asset:///images/actions/icon_play.png"
            enabled: song != null
            ActionBar.placement: ActionBarPlacement.OnBar
            shortcuts: Shortcut {
                key: "p"
            } 
            onTriggered: {
                if(song) {
                    showPlayerView()
                    app.player.play(song)
                }
            }
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            navigationPane: songView.navigationPane
        },
        SameArtistActionItem {
            currentSong: song
            navigationPane: songView.navigationPane
        },
        AddFavouriteActionItem {
            currentSong: song
        },
        RemoveFavouriteActionItem {
            currentSong: song
        },
        SaveSongActionItem{
            songTitle: {
                if(song != null) { 
                    return song.title 
                } else { 
                    return "" 
                }
            }
            currentSong: {
                if(song != null) { 
                    return song.fileName 
                } else { 
                    return "" 
                }
            }
        },
        OpenSongActionItem{},
        AppWorldActionItem{}
    ]
}
