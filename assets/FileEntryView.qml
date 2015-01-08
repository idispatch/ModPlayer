import bb.cascades 1.0
import player 1.0
import "functions.js" as Global

Container {
    id: fileEntryView

    property string fileIcon
    property string fileName
    property string fileType
    property int    fileSize
    property bool   isDirectory
    property int    itemIndex

    background: ((itemIndex % 2 == 1) ? Color.create("#f3f6f9") : Color.White)

    leftPadding: 20
    rightPadding: 20
    topPadding: 12
    bottomPadding: 14

    HorizontalContainer {
        horizontalAlignment: HorizontalAlignment.Fill
        ImageView {
            imageSource: fileEntryView.fileIcon
            preferredHeight: 80
            preferredWidth: 80
            verticalAlignment: VerticalAlignment.Center
            rightMargin: 20
        }
        VerticalContainer {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Fill
            layoutProperties: StackLayoutProperties {
                spaceQuota: 10
            }
            Label {
                text: fileEntryView.fileName
                textStyle {
                    color: Color.Black
                    fontWeight: fileEntryView.isDirectory ? FontWeight.Bold : FontWeight.Normal
                }
            }
            Label {
                text: fileEntryView.fileType
                visible: !fileEntryView.isDirectory
                textStyle.color: Color.Gray
                textStyle.fontSize: FontSize.Small
            }
        }
        HorizontalContainer {
            verticalAlignment: VerticalAlignment.Center
            horizontalAlignment: HorizontalAlignment.Right
            Container {
                leftPadding: 20
                rightPadding: 20
                Label {
                    text: Global.getSizeKb(fileEntryView.fileSize)
                    visible: !fileEntryView.isDirectory
                    textStyle.color: Color.Gray
                    textStyle.fontSize: FontSize.Small
                }
                ImageView {
                    visible: fileEntryView.isDirectory
                    imageSource: "asset:///images/objects/cursor.png"
                    preferredWidth: 32
                    preferredHeight: 48
                }
            }
        }
    }
}