import bb.cascades 1.0
import "functions.js" as Global

GroupContainer {
    property variant song
    VerticalContainer {
        Label {
            visible: song != null && song.artist.length > 0
            text: song != null ? "Artist: <b>" + Global.escapeHtml(song.artist) + "</b>" : ""
            textFormat: TextFormat.Html
        }
        Label {
            visible: song != null && song.genre.length > 0
            text: song != null ? "Genre: <b>" + song.genre + "</b>" : ""
            textFormat: TextFormat.Html
        }
        Label {
            visible: song != null
            text: song != null ? "Format: " + song.format : ""
            textFormat: TextFormat.Html
        }
        Label {
            visible: song != null
            text: song != null ? "Tracker: " + song.tracker : ""
            textFormat: TextFormat.Html
        }
    }
}