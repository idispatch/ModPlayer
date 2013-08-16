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

                SongMainInfo {
                    song: songView.song != null ? songView.song : null
                }

                SongExtendedData {
                    song: songView.song != null ? songView.song : null
                }

                SongPersonalData {
                    song: songView != null ? songView.song : null
                }

                SongOtherStats {
                    song: songView != null ? songView.song : null
                }

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
        },
        ImagePaintDefinition {
            id: background
            repeatPattern: RepeatPattern.Fill
            imageSource: "asset:///images/backgrounds/container_back.amd"
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
