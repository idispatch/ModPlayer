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
                SongArtistInfo {
                    song: songView.song
                }
                SongPersonalInfo {
                    song: songView.song
                }
                
                SongPublicInfo {
                    song: songView.song
                    visible: songView.song && songView.song.isTrackerSong && songView.song.id > 0
                }
                SongParametersInfo {
                    song: songView.song
                    visible: songView.song && songView.song.isTrackerSong
                }
                SongAlbumArt {
                    id: albumArt
                    song: songView.song
                }
                Animation {}
            }
        }
    }
    function load(songId) {
        song = app.player.catalog.resolveModuleById(songId, songView)
        if(song) {
            app.analytics.view(song.id, Global.fileNameOnly(song.fileName))
        }
    }
    function showPlayerView() {
        if(mainTabPane.activePane == navigationPane && navigationPane.top == songView) {
            var view = songPlayer.createObject()
            view.navigationPane = navigationPane
            navigationPane.push(view)
        }
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() { 
            showPlayerView()
        })
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
                    app.player.playlist.clear()
                    app.player.playlist.add(song.id)
                    app.player.playPlaylist()
                }
            }
        },
        AppendPlaylistActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            currentSong: song
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            navigationPane: songView.navigationPane
        },
        PreviousActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        NextActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            navigationPane: songView.navigationPane
        },
        SameArtistActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            currentSong: song
            navigationPane: songView.navigationPane
        },
        AddFavouriteActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            currentSong: song
        },
        RemoveFavouriteActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            currentSong: song
        },
        SaveSongActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
            currentSong: song ? song.fileName : ""
        },
        ImportSongsActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        OpenSongActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
