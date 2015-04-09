import bb.cascades 1.3
import QtQuick 1.0
import player 1.0

HorizontalContainer {
    id: searchArea

    signal search()

    property alias searchTerm: searchCriteria.text
    property alias hintText: searchCriteria.hintText

    leftPadding: 20.0
    rightPadding: 20.0
    topPadding: 20.0
    bottomPadding: 20.0
    leftMargin: 20.0
    rightMargin: 20.0

    TextField {
        id: searchCriteria
        content {
            flags: TextContentFlag.ActiveTextOff | TextContentFlag.EmoticonsOff 
        }
        verticalAlignment: VerticalAlignment.Center
        layoutProperties: StackLayoutProperties {
            spaceQuota: 5
        }
        leftMargin: 10
        rightMargin: 10
        rightPadding: 20.0
        inputMode: TextFieldInputMode.Text
        input {
            submitKey: SubmitKey.Search
            flags: TextInputFlag.SpellCheckOff | 
                   TextInputFlag.PredictionOff | 
                   TextInputFlag.AutoCorrectionOff | 
                   TextInputFlag.AutoCapitalizationOff | 
                   TextInputFlag.AutoPeriodOff | 
                   TextInputFlag.WordSubstitutionOff
            onSubmitted: {
                searchButton.enabled = false
                searchArea.search()
            }
        }
        clearButtonVisible: true
        autoFit: TextAutoFit.None
        onTextChanging: {
            searchButton.enabled = text.length > 0
            searchTimer.startTimer()
        }
    }
    Button {
        id: searchButton
        enabled: false
        imageSource: "asset:///images/actions/icon_search_dark.png"
        verticalAlignment: VerticalAlignment.Center
        leftMargin: 20.0
        rightMargin: leftMargin
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        onClicked: {
            enabled = false
            searchTimer.stop()
            searchArea.search()
        }
    }
    attachedObjects: [
        Timer {
            id: searchTimer
            interval: 1000
            repeat: false
            property string previousText
            function startTimer() {
                stop()
                previousText = searchCriteria.text
                start()
            }
            onTriggered : {
                if(previousText == searchCriteria.text) {
                    searchArea.search()
                } else {
                    previousText = searchCriteria.text
                    start()
                }
            }
        }
    ]
}