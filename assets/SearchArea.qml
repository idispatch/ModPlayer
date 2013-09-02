import bb.cascades 1.0
import player 1.0

HorizontalContainer {
    id: searchArea
    signal search(string term)
    property alias searchTerm: searchCriteria.text 
    leftPadding: 10
    rightPadding: leftPadding
    topPadding: 5
    bottomPadding: topPadding
    attachedObjects: [
        QTimer {
            id: searchTimer
            interval: 400
            singleShot: true
            onTimeout : {
                searchArea.search(searchCriteria.text)
            }
        }
    ]
    TextField {
        id: searchCriteria
        hintText: qsTr("search songs")
        content {
            flags: TextFormat.Plain
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
                searchArea.search(searchCriteria.text)
            }
        }
        clearButtonVisible: true
        autoFit: TextAutofFit.None
        onTextChanging: {
            if(!searchTimer.active) {
                searchTimer.start()
            }
        }
    }
    Button {
        imageSource: "asset:///images/actions/icon_search_dark.png"
        verticalAlignment: VerticalAlignment.Center
        leftMargin: 10
        rightMargin: leftMargin
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        onClicked: {
            searchArea.search(searchCriteria.text)
        }
    }
}