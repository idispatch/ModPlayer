import bb.cascades 1.0
import "functions.js" as Global

VerticalContainer {
    property variant song

    background: backgroundImagePaint.imagePaint

    topPadding: 12
    bottomPadding: 12
    leftPadding: 16
    rightPadding: 16

    horizontalAlignment: HorizontalAlignment.Fill

    topMargin: 16
    bottomMargin: 16

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
    
    attachedObjects: [
        ImagePaintDefinition {
            id: backgroundImagePaint
            repeatPattern: RepeatPattern.Fill
            imageSource: "asset:///images/backgrounds/container_back.amd"
        }
    ]
}