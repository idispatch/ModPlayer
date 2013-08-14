import bb.cascades 1.0

Sheet {
    id: helpRoot
    
    onClosed: {
        helpRoot.destroy()
    }
    
    Page {
        titleBar: TitleBar {
            title: qsTr("Help")
            visibility: ChromeVisibility.Visible
            dismissAction: ActionItem {
                title: qsTr("Close")
                onTriggered: {
                    helpRoot.close();
                }
            }
        }
        ScrollView {
            Container {
                layout: StackLayout {
                    orientation: LayoutOrientation.TopToBottom
                }
                
                leftPadding: 30
                rightPadding: 30
                topPadding: 20
                bottomPadding: 20
                
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p><b>ModPlayer</b> is a tracker music player.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p><b>ModPlayer</b> plays tracker music songs in multiple formats available from <a href='http://www.modarchive.org'>www.modarchive.org</a> site - <i>The Mod Archive</i>.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p><b>ModPlayer</b> has a library of most popular, most downloaded and most scored songs from <i>The Mod Archive</i> site on your device, but does not store all the songs on your device.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p><b>ModPlayer</b> keeps a cache of most recent songs you listened to, and automatically downloads the songs to your device from <i>The Mod Archive</i> site once you start playing them.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p>The <b>ModPlayer</b> allows you to listen to the songs you liked while you are <i>offline</i> when the song is <i>already in cache</i>, you also don't have to download it again.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p>Song cache on you device is limited by the amount of songs you recently listened to, and will automatically delete the older songs once the cache size growth too large.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p>You can configure the song cache size and song count it keeps in <b>ModPlayer</b> settings.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p>You can configure <b>ModPlayer</b> various options, such as <i>mixing frequency</i>, <i>stereo/mono</i> playback, <i>mixer quality</i> and more.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p>You can add the songs from the <b>ModPlayer</b> library to the list of your favourite songs and listen to them later.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p><b>ModPlayer</b> knows the the songs you listened to and you can locate them in the <i>\"Recent\"</i>, <i>\"Most Played\"</i> and <i>\"My Favourite\"</i> tabs.</p>")
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p>There is thousands of songs <b>ModPlayer</b> has in its library, and I hope you will enjoy some songs. A good start is to look in <i>\"Downloads\"</i>, <i>\"Favourited\"</i> and <i>\"Scored\"</i> tabs.</p>" )
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p>These are the songs that were downloaded, rated and favourited by other users of <i>The Mod Archive</i> site and it is quite possible you will also enjoy them.</p>" )
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p><b>ModPlayer</b> is using the <a href='http://www.modplug.com/'>modplug</a> library, one of the best libraries to provide high quality tracker song modules mixing and playback.</p>" )
                }
                Label {
                    multiline: true
                    textFormat: TextFormat.Html
                    topMargin: 2
                    bottomMargin: topMargin
                    text: qsTr("<p>I made <b>ModPlayer</b> for fun and hope you will enjoy it too :)</p>" )
                }
            }
        }
    }
}