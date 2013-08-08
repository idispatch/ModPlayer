import bb.cascades 1.1

Page {
    property int formatId
    property string formatName
    
    property int genreId
    property string genreName
    
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
            return "All Songs"
        }
        appearance: TitleBarAppearance.Branded
        kind: TitleBarKind.Default
    }
    
    Container {
        ListView {
            id: songs
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
                        status: ListItemData.downloads
                    }
                }
            ]
            
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songView.createObject()
                view.navigationPane = navigationPane
                console.log("ModId: " + chosenItem.modId)
                view.load(chosenItem.modId)
                navigationPane.push(view)
            }
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
    
    attachedObjects: [
        ComponentDefinition {
            id: songView
            source: "SongView.qml"
        }
    ]
    
    actions: [ 
        ActionItem {
            title: "Player"
            imageSource: "asset:///images/icon_player.png"
            ActionBar.placement: ActionBarPlacement.OnBar
            shortcuts: Shortcut {
                key: "p"
            } 
            onTriggered: {
                var view = songPlayer.createObject()
                view.navigationPane = navigationPane
                navigationPane.push(view)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songPlayer
                    source: "SongPlayer.qml"
                }
            ]
        }
    ]
}
