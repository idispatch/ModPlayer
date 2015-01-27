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
                    if(!app.player.sleepTimer.timerActive) {
                        app.player.sleepTimer.start()
                    }
                    sleepTimerSheet.close()
                }
            } 
        }
        Container {
            layout: DockLayout {}
            WallpaperView {}
            Container {
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                leftPadding: 20
                rightPadding: 20
                topPadding: 20
                bottomPadding: 20
                ScrollView {
                    horizontalAlignment: HorizontalAlignment.Fill
                    verticalAlignment: VerticalAlignment.Fill
                    
                    GroupContainer {
                        topPadding: 40
                        bottomPadding: 40
                        leftPadding: 60
                        rightPadding: 60
                        BlackLabel {
                            horizontalAlignment: HorizontalAlignment.Center
                            text: app.player.sleepTimer.timerActive ? qsTr("Playback will stop in %1").arg(app.player.sleepTimer.status) + Retranslate.onLanguageChanged : qsTr("Stop playback in %1 minute(s)").arg(app.player.sleepTimer.sleepTimeout) + Retranslate.onLanguageChanged
                            bottomMargin: 40
                        }
                        Container {
                            horizontalAlignment: HorizontalAlignment.Center
                            layout: DockLayout {}
                            leftPadding: 20
                            rightPadding: 20
                            topPadding: 20
                            bottomPadding: 20
                            Clock {
                                horizontalAlignment: HorizontalAlignment.Fill
                                verticalAlignment: VerticalAlignment.Fill
                                clockTime: app.player.sleepTimer.sleepRemaining * 6 
                                setupMode: !app.player.sleepTimer.timerActive
                            }
                        }
                        Slider {
                            horizontalAlignment: HorizontalAlignment.Center
                            topMargin: 60
                            bottomMargin: 60
                            fromValue: 1
                            toValue: 60
                            enabled: !app.player.sleepTimer.timerActive
                            value: app.player.sleepTimer.sleepTimeout
                            onImmediateValueChanged: {
                                app.player.sleepTimer.sleepTimeout = immediateValue
                            }
                        }
                        PlusFeature {
                            extendedVersion: Button {
                                text: app.player.sleepTimer.timerActive ? qsTr("Cancel Timer") + Retranslate.onLanguageChanged : qsTr("Start Timer") + Retranslate.onLanguageChanged
                                horizontalAlignment: HorizontalAlignment.Center
                                onClicked: {
                                    if(app.isExtendedVersion) {
                                        if(app.player.sleepTimer.timerActive) {
                                            app.player.sleepTimer.cancel()
                                        } else {
                                            app.player.sleepTimer.start()
                                        }
                                    } else {
                                        app.pleaseBuy()
                                    }
                                }
                            } 
                        }
                    }
                }
            }
            SleepTimerDisplay {}
        }
    }
}