import bb.cascades 1.0

GroupContainer {
    property alias title: titleField.text
    property alias description: descriptionField.text
    property alias text: textField.text

    property alias upperStatus: statusUpperField.text
    property alias middleStatus: statusMiddleField.text
    property alias lowerStatus: statusLowerField.text
    property alias imageSource: imageView.imageSource
    
    property alias favourite: favouriteImage.visible

    leftPadding: 10
    rightPadding: 10

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
                        multiline: true
                        textStyle {
                            base: SystemDefaults.TextStyles.BodyText
                            fontWeight: FontWeight.W100
                            color: Color.Black
                        }
                    }
                    Label {
                        id: textField
                        visible: text.length > 0
                        horizontalAlignment: HorizontalAlignment.Left
                        textStyle {
                            base: SystemDefaults.TextStyles.SmallText
                            fontWeight: FontWeight.W100
                            fontStyle: FontStyle.Italic
                            color: Color.Black
                        }
                    }
                }
                ImageView {
                    id: favouriteImage
                    imageSource: "asset:///images/badges/badge_myfavourite.png"
                    horizontalAlignment: HorizontalAlignment.Right
                    verticalAlignment: VerticalAlignment.Center
                    scalingMethod: ScalingMethod.None
                    loadEffect: ImageViewLoadEffect.FadeZoom
                    preferredWidth: 32
                    preferredHeight: 32
                    minWidth: 32
                    maxWidth: 32
                    minHeight: 32
                    maxHeight: 32
                }
                VerticalContainer {
                    verticalAlignment: VerticalAlignment.Center
                    Container {
                        visible: upperStatus.length > 0
                        horizontalAlignment: HorizontalAlignment.Right
                        background: upperInfoPaint.imagePaint
                        bottomMargin: 10
                        topPadding: 4
                        bottomPadding: 8
                        leftPadding: 20
                        rightPadding: 16
                        attachedObjects: [
                            ImagePaintDefinition {
                                id: upperInfoPaint
                                repeatPattern: RepeatPattern.Fill
                                imageSource: "asset:///images/backgrounds/infoback.amd"
                            }
                        ]
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
                    }
                    Container {
                        visible: middleStatus.length > 0
                        horizontalAlignment: HorizontalAlignment.Right
                        background: middleInfoPaint.imagePaint
                        topMargin: 10
                        bottomMargin: 10
                        topPadding: 4
                        bottomPadding: 8
                        leftPadding: 20
                        rightPadding: 16
                        attachedObjects: [
                            ImagePaintDefinition {
                                id: middleInfoPaint
                                repeatPattern: RepeatPattern.Fill
                                imageSource: "asset:///images/backgrounds/infoback.amd"
                            }
                        ]
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
                    }
                    Container {
                        visible: lowerStatus.length > 0
                        horizontalAlignment: HorizontalAlignment.Right
                        background: lowerInfoPaint.imagePaint
                        topMargin: 10
                        topPadding: 4
                        bottomPadding: 8
                        leftPadding: 20
                        rightPadding: 16
                        attachedObjects: [
                            ImagePaintDefinition {
                                id: lowerInfoPaint
                                repeatPattern: RepeatPattern.Fill
                                imageSource: "asset:///images/backgrounds/infoback.amd"
                            }
                        ]
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
}
