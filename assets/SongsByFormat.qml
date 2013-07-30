import bb.cascades 1.1

Page {
    property int formatId;
    property string formatName;

    objectName: "SongsByFormat" 
    
    Container {
        Label {
            text: "Id: " + formatId
        }
        Label {
            text: "Name: " + formatName
        }
        ListView {
            id: songsByFormatList
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
                        title: ListItemData.name
                        status: ListItemData.id
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
    
    function loadSongs() {
        console.debug("Selecting songs by format: " + formatId + " (" + formatName + ")")
        var data = app.player.catalog.findSongsByFormatId(formatId)
        songsByFormatList.dataModel = data
    }
}
