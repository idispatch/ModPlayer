import bb.cascades 1.3
import QtQuick 1.0
import player 1.0

HorizontalContainer {
    id: searchArea

    signal search(string term)

    property string searchTerm
    property string hintText

    leftPadding: 20.0
    rightPadding: 20.0
    topPadding: 20.0
    bottomPadding: 20.0
    attachedObjects: [
        Timer {
            id: searchTimer
            interval: 1000
            repeat: false
            property string previousSearchTerm
            onTriggered : {
                if(previousSearchTerm == searchArea.searchTerm) {
                    stop()
                    searchButton.enabled = false
                    searchArea.search(searchArea.searchTerm)
                } else {
                    previousSearchTerm = searchArea.searchTerm
                    start()
                }
            }
        }
    ]
    leftMargin: 20.0
    rightMargin: 20.0
    TextField {
        id: searchCriteria
        text: searchArea.searchTerm
        hintText: searchArea.hintText
        content {
            flags: TextContentFlag.ActiveTextOff | TextContentFlag.EmoticonsOff 
        }
        verticalAlignment: VerticalAlignment.Center
        layoutProperties: StackLayoutProperties {
            spaceQuota: 5
        }
        leftMargin: 10
        rightMargin: leftMargin
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
                searchArea.search(searchArea.searchTerm)
            }
        }
        clearButtonVisible: true
        autoFit: TextAutoFit.None
        onTextChanging: {
            searchButton.enabled = text.length > 0
            if(!searchTimer.running) {
                searchTimer.start()
            }
        }
        rightPadding: 20.0
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
            searchArea.search(searchArea.searchTerm)
        }
    }
}