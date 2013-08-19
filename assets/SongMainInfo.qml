import bb.cascades 1.0
import "functions.js" as Global

GroupContainer {
    id: songMainInfo
    property variant song
    HorizontalContainer {
        horizontalAlignment: HorizontalAlignment.Fill
        SongIconView {
            song: songMainInfo.song
        }
        VerticalContainer {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 10
            }
            Label {
                text: song ? "File: <b>" + song.fileName + "</b>" : ""
                textFormat: TextFormat.Html
                textStyle.color: Color.Black
            }
            Label {
                text: song ? "Size: <b>" + Global.getSizeKb(song.fileSize) + "</b>" : ""
                textFormat: TextFormat.Html
                textStyle.color: Color.Black
            }
            Label {
                text: song ? "Length: <b>" + song.songLengthText + "</b>" : ""
                textFormat: TextFormat.Html
                textStyle.color: Color.Black
            }
            Label {
                text: song ? "Title: <b>" + Global.escapeHtml(song.title) + "</b>" : ""
                textFormat: TextFormat.Html
                textStyle.color: Color.Black
            }
        }
    }
}
