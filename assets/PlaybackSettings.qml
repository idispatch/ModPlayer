import bb.cascades 1.0
import bb.system 1.0
import player 1.0

HorizontalContainer {
    visible: (app.player.currentSong.songLoaded && app.player.currentSong.isTrackerSong) || !app.player.currentSong.songLoaded
    horizontalAlignment: HorizontalAlignment.Center
    topPadding: 20
    ImageView {
        verticalAlignment: VerticalAlignment.Center
        scalingMethod: ScalingMethod.Fill
        loadEffect: ImageViewLoadEffect.None
        imageSource: {
            if(app.player.playback.configuration.frequency > 44000) return "asset:///images/indicators/44khz.png"
            if(app.player.playback.configuration.frequency > 22000) return "asset:///images/indicators/22khz.png"
            if(app.player.playback.configuration.frequency > 11000) return "asset:///images/indicators/11khz.png"
            return "asset:///images/indicators/44khz.png"
        }
    }
    ImageView {
        verticalAlignment: VerticalAlignment.Center
        scalingMethod: ScalingMethod.Fill
        loadEffect: ImageViewLoadEffect.None
        imageSource: {
            if(app.player.playback.configuration.sampleSize == 32) return "asset:///images/indicators/32bit.png"
            if(app.player.playback.configuration.sampleSize == 16) return "asset:///images/indicators/16bit.png"
            return "asset:///images/indicators/8bit.png"
        }
    }
    ImageView {
        verticalAlignment: VerticalAlignment.Center
        scalingMethod: ScalingMethod.Fill
        loadEffect: ImageViewLoadEffect.None
        imageSource: {
            if(app.player.playback.configuration.stereo) return "asset:///images/indicators/stereo.png"
            return "asset:///images/indicators/mono.png"
        }
    }
    ImageView {
        visible: app.player.playback.configuration.noiseReductionEnabled
        verticalAlignment: VerticalAlignment.Center
        scalingMethod: ScalingMethod.Fill
        loadEffect: ImageViewLoadEffect.None
        imageSource: "asset:///images/indicators/highquality.png"
    }
    ImageView {
        visible: app.player.playback.configuration.surroundEnabled
        verticalAlignment: VerticalAlignment.Center
        scalingMethod: ScalingMethod.Fill 
        loadEffect: ImageViewLoadEffect.None
        imageSource: "asset:///images/indicators/surround.png"
    }
    ImageView {
        visible: app.player.playback.configuration.bassEnabled
        verticalAlignment: VerticalAlignment.Center
        scalingMethod: ScalingMethod.Fill
        loadEffect: ImageViewLoadEffect.None
        imageSource: "asset:///images/indicators/xbass.png"
    }
    ImageView {
        visible: app.player.playback.configuration.reverbEnabled
        verticalAlignment: VerticalAlignment.Center
        scalingMethod: ScalingMethod.Fill
        loadEffect: ImageViewLoadEffect.None
        imageSource: "asset:///images/indicators/reverb.png"
    }
    ImageView {
        
        verticalAlignment: VerticalAlignment.Center
        scalingMethod: ScalingMethod.Fill
        loadEffect: ImageViewLoadEffect.None
        imageSource: {
            if(app.player.playback.configuration.resamplingMode == 1) return "asset:///images/indicators/linear.png" 
            if(app.player.playback.configuration.resamplingMode == 2) return "asset:///images/indicators/spline.png"
            if(app.player.playback.configuration.resamplingMode == 3) return "asset:///images/indicators/fir.png"
            return "asset:///images/indicators/nearest.png"
        }
    }
}