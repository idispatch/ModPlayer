import bb.cascades 1.0

HorizontalContainer {
    id: searchArea
    signal search(string term)
    property alias searchTerm: searchCriteria.text 
    leftPadding: 10
    rightPadding: leftPadding
    topPadding: 5
    bottomPadding: topPadding
    TextField {
        id: searchCriteria
        hintText: qsTr("search")
        verticalAlignment: VerticalAlignment.Center
        layoutProperties: StackLayoutProperties {
            spaceQuota: 5
        }
        leftMargin: 10
        rightMargin: leftMargin
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