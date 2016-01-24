import bb.cascades 1.3
import player 1.0
import "functions.js" as Global

Page {
    id: internetRadioPage
    property variant navigationPane
    property alias channelList : internetRadioList.channelList
    property string playlistURL
    titleBar: PlayerTitleBar {
        id: titleBar
        title: qsTr("Select Internet Radio Channel") + Retranslate.onLanguageChanged
        kind: TitleBarKind.FreeForm
        kindProperties: FreeFormTitleBarKindProperties {
            TitleBarText {
                title: titleBar.title
            }
        }
    }
    Container {
        layout: DockLayout {}
        WallpaperView {}
        Container {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
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
                            title: ListItem.data.name ? ListItem.data.name : ListItem.data.playlist
                            description: ListItem.data.description ? ListItem.data.description : title
                            imageSource: Global.getRadioIcon(ListItem.view.channelList)
                            implicitLayoutAnimationsEnabled: false
                            function viewInitialized(initialized) {
                                if(initialized) {
                                    scaleX = 0
                                    scaleY = 0
                                    listItemAnimation.play()
                                }
                            }
                            onCreationCompleted: {
                                ListItem.initializedChanged.connect(viewInitialized)
                            }
                            animations: [
                                ParallelAnimation {
                                    id: listItemAnimation
                                    FadeTransition {
                                        fromOpacity: 0.0
                                        toOpacity: 1.0
                                        duration: 600
                                        easingCurve: StockCurve.Linear
                                    }
                                    ScaleTransition {
                                        toX: 1.0
                                        toY: 1.0
                                        fromX: 0.0
                                        fromY: 0.0
                                        duration: 600
                                        easingCurve: StockCurve.CubicInOut
                                    }
                                }
                            ]
                        }
                    }
                ]
                onTriggered: {
                    var chosenItem = dataModel.data(indexPath)
                    playlistURL = chosenItem.playlist

                    app.player.statusText = qsTr("Tuning Internet Radio") + Retranslate.onLanguageChanged
                    app.player.currentSong.title = "Internet Radio";
                    app.player.currentSong.iconPath = Global.getRadioIcon(playlistURL)

                    showPlayer()

                    app.analytics.selectRadio(playlistURL)
                    app.player.radio.download(playlistURL)
                }
            }
        }
        ListViewItemIndicator {
            id: indicatorView
        }
        ListViewItemSelector {
            id: indexLettersView
            onItemSelected: {
                indicatorView.text = item
                var indexPath = dataModel.lowerBound([item])
                indexPath.pop()
                internetRadioList.scrollToItem(indexPath, app.scrollAnimationType)
            }
        }
        SleepTimerDisplay {}
        PlaylistControl {
            navigationPane: internetRadioPage.navigationPane
        }
    }
    function load() {
        var model
        if(/difm|diforfree/.test(channelList)) {
            model = app.player.catalog.findDigitallyImported()
        } else if(/skyfm/.test(channelList)) {
            model = app.player.catalog.findSkyFm()
        } else if(/jazzradio/.test(channelList)) {
            model = app.player.catalog.findJazzRadio()
        } else if(/rockradio/.test(channelList)) {
            model = app.player.catalog.findRockRadio()
        } else if(/fresca/.test(channelList)) {
            model = app.player.catalog.findFrescaRadio()
        } else if(/[\/.]bbc/.test(channelList)) {
            model = app.player.catalog.findBbcRadio()
        } else {
            model = null
        }
        internetRadioList.dataModel = model
        indexLettersView.dataModel = model
    }
    function unload() {
    }
    function showPlayer() {
        var view = songPlayer.createObject()
        if(view) {
          view.navigationPane = navigationPane
          navigationPane.push(view)
        }
    }
    function addBuyButton() {
        if(!app.isExtendedVersion) {
            var buyActionItem = buyAppComponentDefinition.createObject()
            if(buyActionItem) {
                addAction(buyActionItem, ActionBarPlacement.InOverflow)
            }
        }
    }
    onCreationCompleted: {
        var thisMainTabPane = mainTabPane
        var thisObject = internetRadioPage
        app.player.requestPlayerView.connect(function() {
            if(thisMainTabPane.activePane == thisObject.navigationPane && thisObject.navigationPane.top == thisObject) {
                thisObject.showPlayer()
            }
        })
        app.player.radio.downloadFinished.connect(function(playlist, result) {
            if(thisObject.playlistURL == playlist && result.length > 0) {
                app.analytics.playRadio(playlist)
                var iconURL = Global.getRadioIcon(channelList)
                console.log('Using stream candidates: ' + result)
                var candidateIndex = Math.floor(Math.random() * result.length)
                var candidateURL = result[candidateIndex]
                console.log('Using stream URL: ' + candidateURL)
                app.player.playRadio(candidateURL, iconURL)
            }
        })
        addBuyButton()
    }
    attachedObjects: [
        ComponentDefinition {
            id: buyAppComponentDefinition
            source: "BuyActionItem.qml"
        },
        ComponentDefinition {
            id: songPlayer
            source: "SongPlayer.qml"
        }
    ]
    actions: [
        PlayerActionItem {
            ActionBar.placement: ActionBarPlacement.Signature
            navigationPane: internetRadioPage.navigationPane
        },
        PauseActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        ImportSongsActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        OpenSongActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        EqualizerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        SleepTimerActionItem {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        SettingsMenuAction {
            ActionBar.placement: ActionBarPlacement.InOverflow
        },
        AppWorldActionItem{
            ActionBar.placement: ActionBarPlacement.InOverflow
        }
    ]
}
