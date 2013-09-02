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
                visible: image!=null
                horizontalAlignment: HorizontalAlignment.Left
                verticalAlignment: VerticalAlignment.Center
                scalingMethod: ScalingMethod.Fill
                loadEffect: ImageViewLoadEffect.FadeZoom
                preferredWidth: 128
                preferredHeight: 128
                minWidth: 128
                maxWidth: 128
                minHeight: 128
                maxHeight: 128
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
                        visible: text.length > 0
                        horizontalAlignment: HorizontalAlignment.Left
                        textStyle {
                            base: SystemDefaults.TextStyles.PrimaryText
                            color: Color.Black
                        }
                    }
                    Label {
                        id: descriptionField
                        visible: text.length > 0
                        horizontalAlignment: HorizontalAlignment.Left
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                            fontWeight: FontWeight.W100
                            color: Color.Black
                        }
                    }
                }
                VerticalContainer {
                    verticalAlignment: VerticalAlignment.Center
                    Label {
                        id: statusUpperField
                        visible: text.length > 0
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle {
                            base: SystemDefaults.TextStyles.SubtitleText
                            fontWeight: FontWeight.W100
                            fontStyle: FontStyle.Italic
                            color: Color.Black
                        }
                    }
                    Label {
                        id: statusMiddleField
                        visible: text.length > 0
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle {
                            base: SystemDefaults.TextStyles.SubtitleText
                            fontWeight: FontWeight.W100
                            fontStyle: FontStyle.Italic
                            color: Color.Black
                        }
                    }
                    Label {
                        id: statusLowerField
                        visible: text.length > 0
                        horizontalAlignment: HorizontalAlignment.Right
                        textStyle {
                            base: SystemDefaults.TextStyles.SubtitleText
                            fontWeight: FontWeight.W100
                            fontStyle: FontStyle.Italic
                            color: Color.Black
                        }
                    }
                }
            }
        }
    }
}
