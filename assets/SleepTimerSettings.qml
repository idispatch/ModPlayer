import bb.cascades 1.0
import bb.data 1.0
import player 1.0

Sheet {
    id: sleepTimerSheet
    onClosed: {
        sleepTimerSheet.destroy()
    }
    Page {
        titleBar: PlayerTitleBar {
            title: qsTr("Sleep Timer") + Retranslate.onLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Close") + Retranslate.onLanguageChanged
                onTriggered: {
                    sleepTimerSheet.close()
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Ok") + Retranslate.onLanguageChanged
                onTriggered: {
                    if(!app.player.sleepTimerActive) {
                        app.player.startSleepTimer()
                    }
                    sleepTimerSheet.close()
                }
            } 
        }
        Container {
            layout: DockLayout {}
            WallpaperView {}
            Container {
                topPadding: 20
                bottomPadding: 20
                leftPadding: 10
                rightPadding: 10
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                GroupContainer {
                    topPadding: 40
                    bottomPadding: 40
                    leftPadding: 60
                    rightPadding: 60
                    BlackLabel {
                        horizontalAlignment: HorizontalAlignment.Center
                        text: app.player.sleepTimerActive ? qsTr("Playback will stop in %1 minute(s)").arg(app.player.sleepTimeout) + Retranslate.onLanguageChanged : qsTr("Stop playback in %1 minute(s)").arg(app.player.sleepTimeout) + Retranslate.onLanguageChanged
                    }
                    Slider {
                        topMargin: 60
                        bottomMargin: 60
                        fromValue: 1
                        toValue: 60
                        enabled: !app.player.sleepTimerActive
                        value: app.player.sleepTimeout
                        onImmediateValueChanged: {
                            app.player.sleepTimeout = immediateValue
                        }
                    }
                    Button {
                        text: app.player.sleepTimerActive ? qsTr("Cancel Timer") + Retranslate.onLanguageChanged : qsTr("Start Timer") + Retranslate.onLanguageChanged
                        horizontalAlignment: HorizontalAlignment.Center
                        onClicked: {
                            if(app.player.sleepTimerActive) {
                                app.player.cancelSleepTimer()
                            } else {
                                app.player.startSleepTimer()
                            }
                        }
                    }
                }
            }
        }
    }
}