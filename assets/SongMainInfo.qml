import bb.cascades 1.0
import "functions.js" as Global

HorizontalContainer {
    id: songMainInfo
    property variant song

    background: backgroundImagePaint.imagePaint

    topPadding: 12
    bottomPadding: 12
    leftPadding: 16
    rightPadding: 16

    horizontalAlignment: HorizontalAlignment.Fill

    topMargin: 16
    bottomMargin: 16

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
    
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundImagePaint
            repeatPattern: RepeatPattern.Fill
            imageSource: "asset:///images/backgrounds/container_back.amd"
        }
    ]
}