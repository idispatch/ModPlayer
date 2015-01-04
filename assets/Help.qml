import bb.cascades 1.0
import player 1.0

Sheet {
    id: helpRoot
    onOpened: {
        app.analytics.help(1)
    }
    onClosed: {
        app.analytics.help(0)
        helpRoot.destroy()
    }
    Page {
        titleBar: PlayerTitleBar {
            title: qsTr("ModPlayer Help") + Retranslate.onLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Close") + Retranslate.onLanguageChanged
                onTriggered: {
                    helpRoot.close();
                }
            }
        }
        actions: [
            BuyActionItem {
                ActionBar.placement: ActionBarPlacement.OnBar
            },
            AppWorldActionItem{
                ActionBar.placement: ActionBarPlacement.OnBar
            }
        ]
        ScrollView {
            horizontalAlignment: HorizontalAlignment.Fill
            verticalAlignment: VerticalAlignment.Fill
            VerticalContainer {

                leftPadding: 30
                rightPadding: 30
                topPadding: 20
                bottomPadding: 20

                Animation {}

                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("<b>ModPlayer</b> is a tracker music player.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("<b>ModPlayer</b> plays tracker music songs in multiple formats available from <a href='http://www.modarchive.org'>www.modarchive.org</a> site - <i>The Mod Archive</i>.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("<b>ModPlayer</b> has a library of most popular, most downloaded and most scored songs from <i>The Mod Archive</i> site on your device, but does not store all the songs on your device.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("<b>ModPlayer</b> keeps a cache of most recent songs you listened to, and automatically downloads the songs to your device from <i>The Mod Archive</i> site once you start playing them.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("The <b>ModPlayer</b> allows you to listen to the songs you liked while you are <i>offline</i> when the song is <i>already in cache</i>, you don't have to download it again.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("The song cache on your device is limited by the amount of songs you recently listened to, and will automatically delete the older songs once the cache size grows too large.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("You can configure the song cache size and song count it keeps in <b>ModPlayer</b> settings.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("You can configure various options of <b>ModPlayer</b>, such as <i>mixing frequency</i>, <i>stereo/mono</i> playback, <i>mixer quality</i> and more.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("You can add the songs from the <b>ModPlayer</b> library to the list of your favourite songs and listen to them later.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("<b>ModPlayer</b> knows the songs you listened to and you can locate them in the <i>\"Recent\"</i>, <i>\"Most Played\"</i> and <i>\"My Favourite\"</i> tabs.") + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("There are thousands of songs <b>ModPlayer</b> has in its library, and I hope you will enjoy many songs. A good start is to look in <i>\"Downloads\"</i>, <i>\"Favourited\"</i> and <i>\"Scored\"</i> tabs." ) + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("These are the songs that were downloaded, rated and favourited by other users of <i>The Mod Archive</i> site and it is quite possible you will also enjoy them." ) + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("<b>ModPlayer</b> is using the <a href='http://www.modplug.com/'>modplug</a> library, one of the best libraries that provides high quality tracker song modules mixing and playback." ) + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("I made <b>ModPlayer</b> for fun and hope you will enjoy it too :)" ) + Retranslate.onLanguageChanged
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 20
                    bottomMargin: topMargin
                    text: qsTr("Thank you, contributors: Mr.Time (Chinese translation)." ) + Retranslate.onLanguageChanged
                }
            }
        }
    }
}