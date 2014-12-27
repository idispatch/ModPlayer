import bb.cascades 1.0
import "functions.js" as Global

GroupContainer {
    id: songMainInfo
    property variant song
    onCreationCompleted: {
        mainInfoAnimation.play()
    }
    animations: [
        ScaleTransition {
            id: mainInfoAnimation
            fromY: 0.0
            toY: 1.0
            duration: 400
            easingCurve: StockCurve.CubicInOut
        }
    ]
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
                text: song ? qsTr("File: <b>%1</b>").arg(Global.escapeHtml(Global.fileNameOnly(song.fileName))) : ""
                textFormat: TextFormat.Html
                multiline: true
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
                multiline: true
            }
        }
    }
}
