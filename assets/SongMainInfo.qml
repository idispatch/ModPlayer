import bb.cascades 1.0
import "functions.js" as Global

TGroupContainer {
    id: songMainInfo
    property variant song
    HorizontalContainer {
        horizontalAlignment: HorizontalAlignment.Fill
        SongIconView {
            song: songMainInfo.song
            visible: songMainInfo.song && !songMainInfo.song.isHttpSong
        }
        VerticalContainer {
            layoutProperties: StackLayoutProperties {
                spaceQuota: 10
            }
            BlackLabel {
                topMargin: 0
                bottomMargin: 0
                visible: song && !song.isHttpSong
                text: song ? qsTr("File: <b>%1</b>").arg(Global.fileNameOnly(song.fileName)) : ""
                textFormat: TextFormat.Html
            }
            BlackLabel {
                topMargin: 0
                bottomMargin: 0
                visible: song && !song.isHttpSong
                text: song ? qsTr("Size: <b>%1</b>").arg(Global.getSizeKb(song.fileSize)) : ""
                textFormat: TextFormat.Html
            }
            BlackLabel {
                topMargin: 0
                bottomMargin: 0
                visible: song && !song.isHttpSong
                text: song ? qsTr("Length: <b>%1</b>").arg(song.songLengthText) : ""
                textFormat: TextFormat.Html
            }
            BlackLabel {
                topMargin: 0
                bottomMargin: 0
                text: song ? qsTr("Title: <b>%1</b>").arg(Global.escapeHtml(song.title)) : ""
                textFormat: TextFormat.Html
            }
        }
    }
}
