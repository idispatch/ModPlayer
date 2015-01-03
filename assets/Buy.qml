import bb.cascades 1.0
import bb.system 1.0
import bb.platform 1.0
import player 1.0
import "functions.js" as Global

Sheet {
    id: buySheet
    property int groupSettingIndent: 40
    function buyModPlayerPlus(){
        if(app.isExtendedVersion)
            return
        paymentManager.requestPurchase("", "ModPlayerPlus", "", "", "")
    }
    onOpened: {
        app.analytics.buy(1)
    }
    onClosed: {
        app.analytics.buy(0)
        destroy()
    }
    onCreationCompleted: {
        app.store.retrieveLocalPurchases()
        paymentManager.setConnectionMode(0) // 0=sandbox mode | 1=production mode
        paymentManager.requestExistingPurchases(false)
    }
    attachedObjects: [
        SystemToast {
            id: buyResult
            modality: SystemUiModality.Application
            position: SystemUiPosition.MiddleCenter
            button.label: "Ok"
        },
        PaymentManager {
            id: paymentManager
            onExistingPurchasesFinished: {
                if(reply.errorCode == 0) {
                    for(var i = 0; i < reply.purchases.length; ++i) {
                        app.store.storePurchase(reply.purchases[i]["digitalGoodSku"]);
                        console.log(reply.purchases[i].receipt["digitalGoodSku"]);
                    }
                } else {
                    console.log("Error: " + reply.errorText);
                }
            }
            onPurchaseFinished: {
                if(reply.errorCode == 0) {
                    app.store.storePurchase(reply.digitalGoodSku)
                    buyResult.body = qsTr("You have enabled ModPlayer Plus features.") + Retranslate.onLanguageChanged
                } else {
                    buyResult.body = reply.errorText
                }
                buyResult.exec()
                close()
            }
        }
    ]
    Page {
        titleBar: PlayerTitleBar {
            title: qsTr("ModPlayer Plus") + Retranslate.onLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Close") + Retranslate.onLanguageChanged
                onTriggered: {
                    close();
                }
            }
            acceptAction: ActionItem {
                title: qsTr("Buy") + Retranslate.onLanguageChanged
                enabled: !app.isExtendedVersion
                onTriggered: buyModPlayerPlus()
            } 
        }
        Container {
            layout: DockLayout {}
            WallpaperView {}
            ScrollView {
                opacity: 0.8
                horizontalAlignment: HorizontalAlignment.Fill
                verticalAlignment: VerticalAlignment.Fill
                 VerticalContainer {
                     horizontalAlignment: HorizontalAlignment.Fill
                     verticalAlignment: VerticalAlignment.Fill
                     leftPadding: 20
                     rightPadding: 20
                     topPadding: 20
                     bottomPadding: 20
                     GroupContainer {
                         bottomPadding: 40
                         BlackLabel {
                             text: qsTr("ModPlayer Plus") + Retranslate.onLanguageChanged
                             horizontalAlignment: HorizontalAlignment.Center
                             verticalAlignment: VerticalAlignment.Top
                             textStyle {
                                 base: SystemDefaults.TextStyles.TitleText
                                 fontWeight: FontWeight.Bold
                                 color: Color.Black
                             }
                         }
                         BlackLabel {
                             text: qsTr("Version: <b>%1</b>").arg(app.version) + Retranslate.onLanguageChanged
                             horizontalAlignment: HorizontalAlignment.Center
                             textFormat: TextFormat.Html
                         }
                         Container {
                             id: boughtPane
                             visible: app.isExtendedVersion
                             horizontalAlignment: HorizontalAlignment.Fill
                             topPadding: 20
                             Container {
                                 background: Color.create("#548726")
                                 horizontalAlignment: HorizontalAlignment.Fill
                                 topMargin: 40
                                 bottomMargin: 40
                                 leftPadding: 8
                                 Container {
                                     background: Color.create("#dbffc8")
                                     topPadding: 0
                                     bottomPadding: 10
                                     leftPadding: 20
                                     rightPadding: 10
                                     horizontalAlignment: HorizontalAlignment.Fill
                                     BlackLabel {
                                         text: qsTr("You have enabled ModPlayer Plus features.") + Retranslate.onLanguageChanged
                                         horizontalAlignment: HorizontalAlignment.Fill
                                         textFormat: TextFormat.Html
                                         textStyle.fontSize: FontSize.Small
                                         textStyle.fontWeight: FontWeight.W100
                                         multiline: true
                                     }
                                 }
                             }
                         }
                         Container {
                             id: buyPane
                             visible: !app.isExtendedVersion
                             topPadding: 20
                             BlackLabel {
                                 text: qsTr("ModPlayer Plus edition adds extended features to the free version of ModPlayer.") + Retranslate.onLanguageChanged
                                 horizontalAlignment: HorizontalAlignment.Fill
                                 textFormat: TextFormat.Html
                                 textStyle.fontSize: FontSize.Small
                                 textStyle.fontWeight: FontWeight.W100
                                 multiline: true
                             }
                             BlackLabel {
                                 text: qsTr("If you purchase ModPlayer Plus now you will get all ModPlayer Plus extended features and future updates for free.") + Retranslate.onLanguageChanged
                                 horizontalAlignment: HorizontalAlignment.Fill
                                 textFormat: TextFormat.Html
                                 textStyle.fontSize: FontSize.Small
                                 textStyle.fontWeight: FontWeight.W100
                                 multiline: true
                             }
                             BlackLabel {
                                 text: qsTr("You need to buy ModPlayer Plus extended features only once.") + Retranslate.onLanguageChanged
                                 horizontalAlignment: HorizontalAlignment.Fill
                                 textFormat: TextFormat.Html
                                 textStyle.fontSize: FontSize.Small
                                 textStyle.fontWeight: FontWeight.W100
                                 multiline: true
                             }
                             Container {
                                 background: Color.create("#548726")
                                 horizontalAlignment: HorizontalAlignment.Fill
                                 topMargin: 40
                                 bottomMargin: 40
                                 leftPadding: 8
                                 Container {
                                     background: Color.create("#dbffc8")
                                     topPadding: 10
                                     bottomPadding: 10
                                     leftPadding: 20
                                     rightPadding: 10
                                     horizontalAlignment: HorizontalAlignment.Fill
                                     BlackLabel {
                                         text: qsTr("With <b>ModPlayer Plus</b> you will get:<br/>&#x2022; Custom wallpapers and backgrounds<br/>&#x2022; Import songs from device and SD card<br/>&#x2022; Export songs from ModPlayer cache to SD card or device memory<br/>&#x2022; Open songs from device or SD card<br/>&#x2022; Saving songs to device or SD card<br/>&#x2022; Converting tracker songs to MP3 and saving to device or SD card") + Retranslate.onLanguageChanged
                                         horizontalAlignment: HorizontalAlignment.Fill
                                         textFormat: TextFormat.Html
                                         textStyle.fontSize: FontSize.Small
                                         multiline: true
                                     }
                                 }
                                 onCreationCompleted: infoAnimation.play()
                                 animations: [
                                     TranslateTransition {
                                         id: infoAnimation
                                         duration: 600
                                         fromX: 1000
                                         toX: 0
                                         easingCurve: StockCurve.CubicIn
                                     }
                                 ]
                             }
                             ImageButton {
                                 visible: !app.isExtendedVersion
                                 defaultImageSource: "asset:///images/actions/icon_buy_now.png"
                                 horizontalAlignment: HorizontalAlignment.Center
                                 verticalAlignment: VerticalAlignment.Center
                                 topMargin: 60
                                 onClicked: buyModPlayerPlus()
                                 onCreationCompleted: buyButtonAnimation.play()
                                 animations: [
                                     TranslateTransition {
                                         id: buyButtonAnimation
                                         duration: 600
                                         fromY: 100
                                         toY: 0
                                         easingCurve: StockCurve.CubicIn
                                     }
                                 ]
                            }
                        }
                    }
                }
            }
        }
    }
}