import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

TGroupContainer {
    property variant song
    VerticalContainer {
        BlackLabel {
            visible: song != null && song.artist && song.artist.length > 0
            text: song != null ? qsTr("Artist: <b>%1</b>").arg(Global.escapeHtml(song.artist)) : ""
            textFormat: TextFormat.Html
        }
        BlackLabel {
            visible: song != null && song.genre && song.genre.length > 0
            text: song != null ? qsTr("Genre: <b>%1</b>").arg(Global.escapeHtml(song.genre)) : ""
            textFormat: TextFormat.Html
        }
        BlackLabel {
            visible: song != null
            text: song != null ? qsTr("Format: %1").arg(Global.escapeHtml(song.format)) : ""
            textFormat: TextFormat.Html
        }
        BlackLabel {
            visible: song != null && song.isTrackerSong
            text: song != null ? qsTr("Tracker: %1").arg(Global.escapeHtml(song.tracker)) : ""
            textFormat: TextFormat.Html
        }
    }
}