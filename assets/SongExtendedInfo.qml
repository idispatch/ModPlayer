import bb.cascades 1.0
import "functions.js" as Global

GroupContainer {
    property variant song
    VerticalContainer {
        Label {
            visible: song && song.artist.length > 0
            text: song ? qsTr("Artist: <b>%1</b>").arg(Global.escapeHtml(song.artist)) : ""
            textFormat: TextFormat.Html
            textStyle.color: Color.Black
        }
        Label {
            visible: song && song.genre.length > 0
            text: song ? qsTr("Genre: <b>%1</b>").arg(Global.escapeHtml(song.genre)) : ""
            textFormat: TextFormat.Html
            textStyle.color: Color.Black
        }
        Label {
            visible: song != null
            text: song ? qsTr("Format: %1").arg(song.format) : ""
            textFormat: TextFormat.Html
            textStyle.color: Color.Black
        }
        Label {
            visible: song != null
            text: song ? qsTr("Tracker: %1").arg(Global.escapeHtml(song.tracker)) : ""
            textFormat: TextFormat.Html
            textStyle.color: Color.Black
        }
    }
}