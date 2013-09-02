import bb.cascades 1.0

HorizontalContainer {
    id: searchArea
    signal search(string term)
    onSearch: console.log("searchArea.Searching " + term)
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
        rightMargin: 10
    }
    Button {
        imageSource: "asset:///images/actions/icon_search_dark.png"
        verticalAlignment: VerticalAlignment.Center
        leftMargin: 10
        rightMargin: 10
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1
        }
        onClicked: {
            searchArea.search(searchCriteria.text)
        }
    }
}