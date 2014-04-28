import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: internetRadioPage
    objectName: "internetRadioPage"
    property variant navigationPane
    property alias channelList : internetRadioList.channelList
    titleBar: PlayerTitleBar {
        title: qsTr("Select Channel")
    }
    ViewContainer {
        ListView {
            id: internetRadioList
            property string channelList
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            topPadding: 20
            bottomPadding: topPadding
            leftPadding: 10
            rightPadding: leftPadding
            listItemComponents: [
                ListItemComponent {
                    type: "header"
                    Label {
                        text: ListItem.data
                        textStyle {
                            fontWeight: FontWeight.Bold
                            fontSize: FontSize.Large
                            color: Color.White
                        }
                    }
                },
                ListItemComponent {
                    type: "item"
                    ModPlayerListItem {
                        title: ListItem.data.name
                        description: ListItem.data.description
                        imageSource: {
                            if(ListItem.view.channelList.indexOf("difm") != -1) {
                                return "asset:///images/formats/icon-difm.png"
                            } else if(ListItem.view.channelList.indexOf("skyfm") != -1) {
                                return "asset:///images/formats/icon-skyfm.png"
                            } else if(ListItem.view.channelList.indexOf("jazzradio") != -1) {
                                return "asset:///images/formats/icon-jazzradio.png"
                            } else if(ListItem.view.channelList.indexOf("rockradio") != -1) {
                                return "asset:///images/formats/icon-rockradio.png"
                            }
                            return ""
                        }
                    }
                }
            ]
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                console.log(chosenItem.playlist)
                app.player.play("http://pub4.di.fm:80/di_bassnjackinhouse_aac")
                showPlayerView()
            }
            function showPlayerView() {
                if(mainTabPane.activePane == navigationPane && 
                   navigationPane.top == internetRadioPage) {
                    var view = songPlayer.createObject()
                    view.navigationPane = navigationPane
                    navigationPane.push(view)
                }
            }
        }
    }
    function load() {
        if(channelList.indexOf("difm") != -1) {
            internetRadioList.dataModel = app.player.catalog.findDigitallyImported()
        } else if(channelList.indexOf("skyfm") != -1) {
            internetRadioList.dataModel = app.player.catalog.findSkyFm()
        } else if(channelList.indexOf("jazzradio") != -1) {
            internetRadioList.dataModel = app.player.catalog.findJazzRadio()
        } else if(channelList.indexOf("rockradio") != -1) {
            internetRadioList.dataModel = app.player.catalog.findRockRadio()
        }
    }
    function unload() {
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() {
            if(mainTabPane.activePane == navigationPane && 
               navigationPane.top == internetRadioPage) {
                var view = songPlayer.createObject()
                view.navigationPane = navigationPane
                navigationPane.push(view)
            }
        })
    }
    attachedObjects: [
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
    actions: [
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.OnBar
            navigationPane: internetRadioPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            navigationPane: internetRadioPage.navigationPane
        },
        ImportSongsActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        OpenSongActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
