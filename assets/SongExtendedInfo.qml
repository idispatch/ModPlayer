import bb.cascades 1.0
import "functions.js" as Global

GroupContainer {
    property variant song
    VerticalContainer {
        Label {
            visible: song && song.artist.length > 0
            text: song ? "Artist: <b>" + Global.escapeHtml(song.artist) + "</b>" : ""
            textFormat: TextFormat.Html
            textStyle.color: Color.Black
        }
        Label {
            visible: song && song.genre.length > 0
            text: song ? "Genre: <b>" + song.genre + "</b>" : ""
            textFormat: TextFormat.Html
            textStyle.color: Color.Black
        }
        Label {
            visible: song != null
            text: song ? "Format: " + song.format : ""
            textFormat: TextFormat.Html
            textStyle.color: Color.Black
        }
        Label {
            visible: song != null
            text: song ? "Tracker: " + song.tracker : ""
            textFormat: TextFormat.Html
            textStyle.color: Color.Black
        }
    }
}