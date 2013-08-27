import bb.cascades 1.0

Page {
    id: songView

    property variant song
    property variant navigationPane 

    ViewContainer {
        ScrollView {
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

    function play() {
        if(song) {
            showPlayer()
            app.player.play(song)
        }
    }

    function showPlayer() {
        var view = songPlayer.createObject()
        navigationPane.push(view)
    }

    function load(songId) {
        song = app.player.catalog.resolveModuleById(songId, songView)
        app.analytics.view(song.id, song.fileName)
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
