import bb.cascades 1.3
import "functions.js" as Global

Page {
    id: internetRadioPage
    objectName: "internetRadioPage"
    property variant navigationPane
    property alias channelList : internetRadioList.channelList
    property string playlistURL
    titleBar: PlayerTitleBar {
        title: qsTr("Select Internet Radio Channel")
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
                        favourite: false
                        description: ListItem.data.description
                        imageSource: Global.getRadioIcon(ListItem.view.channelList)
                    }
                }
            ]
            onTriggered: {
                var chosenItem = dataModel.data(indexPath)
                playlistURL = chosenItem.playlist
                
                app.player.statusText = qsTr("Tuning Internet Radio")
                app.player.currentSong.title = "Internet Radio";
                app.player.currentSong.iconPath = Global.getRadioIcon(playlistURL)
                
                showPlayerView()
                
                app.analytics.selectRadio(playlistURL)
                app.player.radio.download(playlistURL)
            }
        }
    }
    function load() {
        var model
        if(channelList.indexOf("difm") != -1) {
            model = app.player.catalog.findDigitallyImported()
        } else if(channelList.indexOf("skyfm") != -1) {
            model = app.player.catalog.findSkyFm()
        } else if(channelList.indexOf("jazzradio") != -1) {
            model = app.player.catalog.findJazzRadio()
        } else if(channelList.indexOf("rockradio") != -1) {
            model = app.player.catalog.findRockRadio()
        } else {
            model = null
        }
        internetRadioList.dataModel = model
    }
    function unload() {
    }
    function showPlayerView() {
        if(mainTabPane.activePane == navigationPane && 
           navigationPane.top == internetRadioPage) {
            var view = songPlayer.createObject()
            view.navigationPane = navigationPane
            navigationPane.push(view)
        }
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
        app.player.radio.downloadFinished.connect(function(playlist,result) {
            if(playlistURL == playlist && result.length > 0) {
                app.analytics.playRadio(playlist)
                var iconURL = Global.getRadioIcon(channelList)
                app.player.playRadio(result[0], iconURL)
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
