import bb.cascades 1.0
import player 1.0

Slider {
    id: songProgress
    visible: app.player.currentSong.songLoaded
    fromValue: 0
    toValue: app.player.currentSong.orders
    value: app.player.currentSong.currentOrder
    horizontalAlignment: HorizontalAlignment.Fill
    onValueChanged: {
        if(app.player.currentSong.currentOrder != value) {
            app.player.seek(value)
        }
    }
}