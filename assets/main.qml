import bb.cascades 1.2

Page {
    Container {
        layout: StackLayout {
        }
        TextField {
            id: modId
            text: ""
            hintText: "Module ID"
        }
        Label {
            id: statusText
            text: app.player.statusText
        }
        Label {
            text: "Cached files: " + app.player.cache.currentFiles
        }
        Label {
            text: {
                var x = app.player.cache.currentSize
                var s
                if(x < 1024) {
                    s = x + " bytes"
                } else { 
                    x /= 1024
                    if(x < 1024) {
                        s = x.toFixed(2) + " kB"
                    } else {
                        x /= 1024
                        s = x.toFixed(2) + " MB"
                    }
                }
                return "Total cached file size: " + s 
            }
        }
        Button {
            text: "Play"
            horizontalAlignment: HorizontalAlignment.Center
            verticalAlignment: VerticalAlignment.Center
            onClicked: {
                console.log(app.player)
                app.player.play(modId.text);
            }
        }
    }
}
