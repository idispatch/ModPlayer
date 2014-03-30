import bb.cascades 1.0
import player 1.0

HorizontalContainer {
    id: searchArea
    signal search(string term)
    property alias searchTerm: searchCriteria.text
    property alias hintText: searchCriteria.hintText 
    leftPadding: 10
    rightPadding: leftPadding
    topPadding: 10
    bottomPadding: topPadding
    attachedObjects: [
        QTimer {
            id: searchTimer
            interval: 1000
            singleShot: true
            property string previousSearchTerm
            onTimeout : {
                if(previousSearchTerm == searchTerm) {
                    stop()
                    searchButton.enabled = false
                    searchArea.search(searchTerm)
                } else {
                    previousSearchTerm = searchTerm
                    start()
                }
            }
        }
    ]
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
                searchArea.search(searchTerm)
            }
        }
        clearButtonVisible: true
        autoFit: TextAutoFit.None
        onTextChanging: {
            searchButton.enabled = text.length > 0
            if(!searchTimer.active) {
                searchTimer.start()
            }
        }
    }
    Button {
        id: searchButton
        enabled: false
        imageSource: "asset:///images/actions/icon_search_dark.png"
        verticalAlignment: VerticalAlignment.Center
        leftMargin: 10
        rightMargin: leftMargin
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        onClicked: {
            enabled = false
            searchArea.search(searchTerm)
        }
    }
}