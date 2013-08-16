import bb.cascades 1.0
import player 1.0

Page {
    id: songListPage
    property int formatId
    property string formatName

    property int genreId
    property string genreName
    
    property int artistId
    property string artistName
    
    property string mode
    property variant navigationPane

    titleBar: TitleBar {
        title: {
            if(mode == 'format') {
                return formatName + " Songs";
            }
            if(mode == 'genre') {
                return genreName + " Songs";
            }
            if(mode == 'artist') {
                return "Songs by " + artistName;
            }
            return "All Songs"
        }
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    ViewContainer {
        ListView {
            id: songs
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            topPadding: 20
            bottomPadding: topPadding
            leftPadding: 10
            rightPadding: leftPadding
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Label {
                        text: ListItem.data
                        textStyle.fontWeight: FontWeight.Bold
                        textStyle.fontSize: FontSize.Large
                        textStyle.color: Color.White
                    }
                },
                ListItemComponent {
                    type: "item"
                    ModPlayerListItem {
                        title: ListItem.data.title
                        description: ListItem.data.fileName
                        upperStatus: {
                            if(ListItem.data.score > 0) {
                                return "score " + ListItem.data.score + " of 10"
                            }
                            return ""
                        }
                        middleStatus: {
                            if(ListItem.data.favourited > 0) {
                                return "favourited " + ListItem.data.favourited + " times"
                            }
                            return ""
                        } 
                        lowerStatus: {
                            if(ListItem.data.downloads > 0) {
                                return ListItem.data.downloads + " downloads"
                            }
                            return ""
                        }  
                        imageSource: ListItem.data.iconPath
                    }
                }
            ]
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songView.createObject()
                view.navigationPane = navigationPane
                view.load(chosenItem.id)
                navigationPane.push(view)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songView
                    source: "SongView.qml"
                }
            ]
        }
    }

    function loadSongsByFormat(formatId, formatName) {
        this.mode = "format"
        this.formatId = formatId
        this.formatName = formatName
        var data = app.player.catalog.findSongsByFormatId(formatId)
        songs.dataModel = data
    }

    function loadSongsByGenre(genreId, genreName) {
        this.mode = "genre"
        this.genreId = genreId
        this.genreName = genreName
        var data = app.player.catalog.findSongsByGenreId(genreId)
        songs.dataModel = data
    }

    function loadSongsByArtist(artistId, artistName) {
        this.mode = "artist"
        this.artistId = artistId
        this.artistName = artistName
        var data = app.player.catalog.findSongsByArtistId(artistId)
        songs.dataModel = data
    }

    actions: [
        PlayerActionItem {
            navigationPane: songListPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
