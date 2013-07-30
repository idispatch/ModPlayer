import bb.cascades 1.1

Page {
    property int genreId;
    property string genreName;
    
    objectName: "SongByGenre" 
    
    Container {
        Label {
            text: "Id: " + genreId
        }
        Label {
            text: "Name: " + genreName
        }
    }
}
