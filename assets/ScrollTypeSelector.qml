import bb.cascades 1.3
import player 1.0

DropDown {
    title: qsTr("Scroll") + Retranslate.onLanguageChanged
    Option {
        text: qsTr("Smooth") + Retranslate.onLanguageChanged
        value: 2
        selected: app.scrollAnimationType == 2
    }
    Option {
        text: qsTr("Fast") + Retranslate.onLanguageChanged
        value: 1
        selected: app.scrollAnimationType == 1
    }
    onSelectedValueChanged: {
        app.scrollAnimationType = selectedValue 
    }
}