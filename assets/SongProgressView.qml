import bb.cascades 1.0
import player 1.0

Slider {
    property bool sliderUpdatedByTouch: false
    id: songProgress
    visible: app.player.currentSong.songLoaded && !app.player.currentSong.isHttpSong
    fromValue: 0
    toValue: app.player.currentSong.orders
    value: app.player.currentSong.currentOrder
    horizontalAlignment: HorizontalAlignment.Fill
    onValueChanged: {
        if(sliderUpdatedByTouch &&
           !app.player.currentSong.isHttpSong && 
           app.player.currentSong.currentOrder != value) {
            app.player.seek(value)
            sliderUpdatedByTouch = false
        }
    }
    onTouch: {
        if(event.isUp()) {
            sliderUpdatedByTouch = true
        }
    }
}