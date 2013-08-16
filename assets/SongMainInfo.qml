import bb.cascades 1.0
import "functions.js" as Global

GroupContainer {
    id: songMainInfo
    property variant song
    HorizontalContainer {
        horizontalAlignment: HorizontalAlignment.Fill
        SongIconView {
            song: songMainInfo.song != null ? songMainInfo.song : null
        }
        VerticalContainer {
            Label {
                text: song != null ? "File: <b>" + song.fileName + "</b>" : ""
                textFormat: TextFormat.Html
            }
            Label {
                text: song != null ? "Size: <b>" + Global.getSizeKb(song.fileSize) + "</b>" : ""
                textFormat: TextFormat.Html
            }
            Label {
                text: song != null ? "Length: <b>" + song.songLengthText + "</b>" : ""
                textFormat: TextFormat.Html
            }
            Label {
                text: song != null ? "Title: <b>" + Global.escapeHtml(song.title) + "</b>" : ""
                textFormat: TextFormat.Html
            }
        }
    }
}
