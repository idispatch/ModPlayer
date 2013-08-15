import bb.cascades 1.0
import "functions.js" as Global

Container {
    id: songExtendedData
    layout: StackLayout {
        orientation: LayoutOrientation.TopToBottom
    }

    property variant song

    Label {
        visible: songExtendedData.song != null && songExtendedData.song.artist.length > 0
        text: songExtendedData.song != null ? "Artist: <b>" + Global.escapeHtml(songExtendedData.song.artist) + "</b>" : ""
        textFormat: TextFormat.Html
    }
    Label {
        visible: songExtendedData.song != null && songExtendedData.song.genre.length > 0
        text: songExtendedData.song != null ? "Genre: <b>" + songExtendedData.song.genre + "</b>" : ""
        textFormat: TextFormat.Html
    }
    Label {
        visible: songExtendedData.song != null
        text: songExtendedData.song != null ? "Format: " + songExtendedData.song.format : ""
        textFormat: TextFormat.Html
    }
    Label {
        visible: songExtendedData.song != null
        text: songExtendedData.song != null ? "Tracker: " + songExtendedData.song.tracker : ""
        textFormat: TextFormat.Html
    }
}