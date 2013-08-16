import bb.cascades 1.0

GroupContainer {
    property alias title: titleField.text
    property alias description: descriptionField.text

    property alias upperStatus: statusUpperField.text
    property alias middleStatus: statusMiddleField.text
    property alias lowerStatus: statusLowerField.text
    property alias imageSource: imageView.imageSource

    leftPadding: 10
    rightPadding: leftPadding 

    VerticalContainer {
        HorizontalContainer {
            ImageView {
                id: imageView
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                preferredWidth: 128
                preferredHeight: 128
                minWidth: preferredWidth
                maxWidth: minWidth
                minHeight: preferredHeight
                maxHeight: minHeight
            }
            HorizontalContainer {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Center
                leftPadding: 10
                rightPadding: 10
                bottomPadding: 5
                VerticalContainer {
                    layoutProperties: StackLayoutProperties {
                        spaceQuota: 10
                    }
                    rightPadding: 20
                    Label {
                        id: titleField
                        text: "Title"
                        horizontalAlignment: HorizontalAlignment.Left
                        textStyle.base: SystemDefaults.TextStyles.PrimaryText
                        textStyle.color: Color.Black
                    }
                    Label {
                        id: descriptionField
                        text: "Description"
                        horizontalAlignment: HorizontalAlignment.Left
                        textStyle.base: SystemDefaults.TextStyles.BodyText
                        textStyle.fontWeight: FontWeight.W100
                        textStyle.color: Color.Black
                    }
                }
                
                VerticalContainer {
                    layout: StackLayout {
                        orientation: LayoutOrientation.TopToBottom
                    }
                    verticalAlignment: VerticalAlignment.Center
                    Label {
                        id: statusUpperField
                        visible: text.length > 0
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.base: SystemDefaults.TextStyles.SubtitleText
                        textStyle.fontWeight: FontWeight.W100
                        textStyle.fontStyle: FontStyle.Italic
                        textStyle.color: Color.Black
                    }
                    Label {
                        id: statusMiddleField
                        visible: text.length > 0
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.base: SystemDefaults.TextStyles.SubtitleText
                        textStyle.fontWeight: FontWeight.W100
                        textStyle.fontStyle: FontStyle.Italic
                        textStyle.color: Color.Black
                    }
                    Label {
                        id: statusLowerField
                        visible: text.length > 0
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle.base: SystemDefaults.TextStyles.SubtitleText
                        textStyle.fontWeight: FontWeight.W100
                        textStyle.fontStyle: FontStyle.Italic
                        textStyle.color: Color.Black
                    }
                }
            }
        }
    }
}
