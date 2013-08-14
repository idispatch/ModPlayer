import bb.cascades 1.0
import player 1.0

Page {
    property int formatId
    property string formatName
    
    property int genreId
    property string genreName
    
    property int artistId
    property string artistName
    
    property string mode
    property variant navigationPane

    objectName: "SongList" 
    
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
    
    Container {
        background: back.imagePaint
        attachedObjects: [
            ImagePaintDefinition {
                id: back
                repeatPattern: RepeatPattern.Fill
                imageSource: "asset:///images/backgrounds/background.png"
            }
        ]
        ListView {
            id: songs
            property variant app_ref: app
            
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Header {
                        title: ListItemData
                    }
                },
                ListItemComponent {
                    type: "item"
                    StandardListItem {
                        title: ListItemData.title
                        description: ListItemData.fileName
                        status: ListItemData.downloads + " downloads"
                        imageSource: ListItem.view.app_ref.getIconPath(ListItemData)
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songView.createObject()
                view.navigationPane = navigationPane
                view.load(chosenItem.modId)
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
            navigationPane: parent.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        } 
    ]
}
