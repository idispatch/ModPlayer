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
                text: song ? qsTr("File: <b>%1</b>").arg(song.fileName) : ""
                textFormat: TextFormat.Html
                textStyle.color: Color.Black
            }
            Label {
                text: song ? qsTr("Size: <b>%1</b>").arg(Global.getSizeKb(song.fileSize)) : ""
                textFormat: TextFormat.Html
                textStyle.color: Color.Black
            }
            Label {
                text: song ? qsTr("Length: <b>%1</b>").arg(song.songLengthText) : ""
                textFormat: TextFormat.Html
                textStyle.color: Color.Black
            }
            Label {
                text: song ? qsTr("Title: <b>%1</b>").arg(Global.escapeHtml(song.title)) : ""
                textFormat: TextFormat.Html
                textStyle.color: Color.Black
            }
        }
    }
}
