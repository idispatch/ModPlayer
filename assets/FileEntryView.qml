import bb.cascades 1.3
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

    background: {
        if(Application.themeSupport.theme.colorTheme.style == VisualStyle.Bright) {
            return ((itemIndex % 2 == 1) ? Color.create("#f3f6f9") : Color.White)
        } else {
            return ((itemIndex % 2 == 1) ? Color.create("#545454") : Color.create("#4e4e4e"))
        }
    }

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
            BlackLabel {
                text: fileEntryView.fileName
                textStyle {
                    fontWeight: fileEntryView.isDirectory ? FontWeight.Bold : FontWeight.Normal
                }
            }
            BlackLabel {
                text: fileEntryView.fileType
                visible: !fileEntryView.isDirectory
                textStyle {
                    color: Color.Gray
                    fontSize: FontSize.Small
                }
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
                    preferredWidth: 24
                    preferredHeight: 40
                }
            }
        }
    }
}