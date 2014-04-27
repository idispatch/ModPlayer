import bb.cascades 1.0
import "functions.js" as Global

Page {
    id: difmPage
    objectName: "difmPage"
    property variant navigationPane
    titleBar: PlayerTitleBar {
        title: qsTr("Select Channel")
    }
    ViewContainer {
        ListView {
            id: difmList
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
                        imageSource: "asset:///images/formats/icon-difm.png"
                    }
                }
            ]
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                var view = songList.createObject()
                view.navigationPane = navigationPane
                navigationPane.push(view)
                view.loadSongsByFormat(chosenItem.id, chosenItem.name)
            }
            attachedObjects: [
                ComponentDefinition {
                    id: songList
                    source: "SongList.qml"
                }
            ]
        }
    }
    function load() {
        if(difmList.dataModel == null || difmList.dataModel.size() == 0) {
            difmList.dataModel = app.player.catalog.findDigitallyImported()
        }
    }
    function unload() {
    }
    onCreationCompleted: {
        app.player.requestPlayerView.connect(function() {
                if(mainTabPane.activePane == navigationPane && 
                   navigationPane.top == difmPage) {
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
            navigationPane: difmPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
            navigationPane: difmPage.navigationPane
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
