import bb.cascades 1.3

HorizontalContainer {
    id: multilineCheckBox

    property alias text: labelText.text
    property alias checked: checkBox.checked
    signal checkedChanged()

    horizontalAlignment: HorizontalAlignment.Fill
    bottomMargin: 40
    Label {
        id: labelText
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }
        textStyle {
            color: Color.Black
            fontSize: FontSize.Medium
        }
        textFormat: TextFormat.Html
        multiline: true
    }
    CheckBox {
        id: checkBox
        horizontalAlignment: HorizontalAlignment.Right
        verticalAlignment: VerticalAlignment.Center
        onCheckedChanged: {
            multilineCheckBox.checkedChanged()
        }
    }
}