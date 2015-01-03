import bb.cascades 1.0
import bb.data 1.0
import player 1.0

Sheet {
    id: colorSheet
    onClosed: destroy()
    Page {
        titleBar: PlayerTitleBar {
            title: qsTr("Select Color") + Retranslate.onLanguageChanged
            dismissAction: ActionItem {
                title: qsTr("Close") + Retranslate.onLanguageChanged
                onTriggered: close()
            }
            acceptAction: ActionItem {
                title: qsTr("Ok") + Retranslate.onLanguageChanged
                onTriggered: {
                    var chosenItem = colorList.dataModel.data(colorList.selected())
                    if(chosenItem) {
                        app.wallpaper.color = colorList.d
                        app.wallpaper.solidColor = true
                    }
                    close()
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
                    attachedObjects: [
                        ArrayDataModel {
                            id: colorDataModel
                        },
                        DataSource {
                            id: dataSource
                            source: "colors.json"
                            onDataLoaded: {
                                colorDataModel.append(data)
                                console.log("Colors: " + colorDataModel.size())
                            }
                        }
                    ]
                    onCreationCompleted: {
                        dataSource.load()
                    }
                    ListView {
                        id: colorList
                        layout: GridListLayout {
                            columnCount: 4
                            horizontalCellSpacing: 16
                            verticalCellSpacing: 16
                        }
                        listItemComponents: [
                            ListItemComponent {
                                ColorListItem {
                                    colorName: ListItem.data.colorName
                                    colorValue: ListItem.data.colorValue
                                }
                            }
                        ]
                        dataModel: colorDataModel
                        onTriggered: {
                            var chosenItem = dataModel.data(indexPath)
                            if(chosenItem) {
                                app.wallpaper.color = chosenItem.colorValue
                                app.wallpaper.solidColor = true
                                colorSheet.close()
                            }
                        }
                    }
                }
            }
        }
    }
}