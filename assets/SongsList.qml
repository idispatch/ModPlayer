import bb.cascades 1.1

Page {
    property int formatId;
    property string formatName;
    
    property int genreId;
    property string genreName;
    
    property string mode;

    objectName: "SongsList" 
    
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
                console.debug("Id: " + chosenItem.id)
                console.debug("Name: " + chosenItem.name)
            }
        }
    }
    
    function loadSongsByFormat(formatId, formatName) {
        this.mode = "format"
        this.formatId = formatId
        this.formatName = formatName
        console.debug("Selecting songs by format: " + this.formatId + " (" + this.formatName + ")")
        var data = app.player.catalog.findSongsByFormatId(formatId)
        songs.dataModel = data
    }
    
    function loadSongsByGenre(genreId, genreName) {
        this.mode = "genre"
        this.genreId = genreId
        this.genreName = genreName
        console.debug("Selecting songs by genre: " + this.genreId + " (" + this.genreName + ")")
        //var data = app.player.catalog.findSongsByFormatId(formatId)
        //songs.dataModel = data
    }
}
