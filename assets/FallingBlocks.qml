import bb.cascades 1.3
import player 1.0

GroupContainer {
    id: fallingBlocks
    property variant navigationPane
    onCreationCompleted: {
        var sceneObject = scene
        var currentSong = app.player.currentSong
        var thisMainTabPane = mainTabPane
        var thisObject = fallingBlocks
        currentSong.isHttpSongChanged.connect(function() {
            if (thisMainTabPane.activePane == thisObject.navigationPane) {
                if (currentSong.isHttpSong && !sceneObject.isRunning) {
                    sceneObject.run()
                }
            }
        })
    }
    Container {
        id: scene
        property bool isRunning: false
        horizontalAlignment: HorizontalAlignment.Center
        verticalAlignment: VerticalAlignment.Fill
        preferredHeight: 500
        preferredWidth: 500
        layout: AbsoluteLayout {}
        attachedObjects: [
            QTimer {
                id: displayTimer
                interval: 26000
                singleShot: false
                onTimeout: { 
                    scene.startFall() 
                }
            },
            ComponentDefinition {
                id: blockComponent
                content: ImageView {
                    id: block
                    imageSource: {
                        var imgs = [
                            "asset:///images/objects/block-yellow.png",
                            "asset:///images/objects/block-green.png",
                            "asset:///images/objects/block-purple.png",
                            "asset:///images/objects/block-blue.png",
                            "asset:///images/objects/block-brown.png",
                            "asset:///images/objects/block-cyan.png",
                            "asset:///images/objects/block-red.png"
                        ]
                        return imgs[blockColor % imgs.length]
                    }
                    property alias px: layoutProps.positionX
                    property alias py: layoutProps.positionY
                    property alias startDelay: fallAnimation.delay
                    property real stayDelay: 3500.0
                    property real fallDelay: 800.0
                    property variant scene
                    property int blockColor: 0
                    
                    layoutProperties: AbsoluteLayoutProperties {
                        id: layoutProps
                    }
                    function play() {
                        translationY = -550;
                        fallAnimation.play()
                    }
                    animations: [
                        SequentialAnimation {
                            id: fallAnimation
                            TranslateTransition {
                                duration: block.fallDelay
                                fromY: -550
                                toY: 0
                                easingCurve: StockCurve.ExponentialOut
                            }
                            TranslateTransition {
                                delay: block.stayDelay
                                duration: block.fallDelay
                                fromY: 0
                                toY: 550
                                easingCurve: StockCurve.ExponentialIn
                            }
                            onEnded: {
                                scene.remove(block)
                                //console.log("remaining: " + scene.controls.length)
                            }
                        }
                    ]
                }
            }
        ]
        function run() {
            isRunning = true
            startFall()
            displayTimer.start()
        }
        function startFall() {
            var letters = [
                [ 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00],
                [ 0x44, /* 010001 */
                  0x6C, /* 011011 */
                  0x54, /* 010101 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x00  /* 000000 */
                ],
                [ 0x38, /* 001110 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x38, /* 001110 */
                  0x00  /* 000000 */
                ],
                [ 0x78, /* 011110 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x78, /* 011110 */
                  0x00  /* 000000 */
                ],
                [ 0x78, /* 011110 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x78, /* 011110 */
                  0x40, /* 010000 */
                  0x40, /* 010000 */
                  0x40, /* 010000 */
                  0x00  /* 000000 */
                ],
                [ 0x40, /* 010000 */
                  0x40, /* 010000 */
                  0x40, /* 010000 */
                  0x40, /* 010000 */
                  0x40, /* 010000 */
                  0x40, /* 010000 */
                  0x7C, /* 011111 */
                  0x00  /* 000000 */
                ],
                [ 0x38, /* 001110 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x7C, /* 011111 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x00  /* 000000 */
                ],
                [ 0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x28, /* 001010 */
                  0x10, /* 000100 */
                  0x10, /* 000100 */
                  0x10, /* 000100 */
                  0x00  /* 000000 */
                ],
                [ 0x7C, /* 011111 */
                  0x40, /* 010000 */
                  0x40, /* 010000 */
                  0x78, /* 011110 */
                  0x40, /* 010000 */
                  0x40, /* 010000 */
                  0x7C, /* 011111 */
                  0x00  /* 000000 */
                ],
                [ 0x78, /* 011110 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x78, /* 011110 */
                  0x48, /* 010010 */
                  0x44, /* 010001 */
                  0x44, /* 010001 */
                  0x00  /* 000000 */
                ] ]
            var fallDelay = 150
            var maxZ = 10
            var maxX = 6
            var maxY = 7
            for (var z = 0; z < maxZ; z ++) {
                for (var y = 0; y < maxY; ++ y) {
                    for (var x = 0; x < maxX; ++ x) {
                        if ((letters[z][y] & (1 << (x + 1))) == 0)
                            continue
                        var obj = blockComponent.createObject()
                        obj.px = scene.preferredWidth / 2 - 25 * x + y * 25
                        obj.py = scene.preferredHeight / 2 + 14 * x + y * 15 - z * 25
                        obj.startDelay = y * maxX * fallDelay / 2 + x * fallDelay + z * maxX * maxY * fallDelay / 4
                        obj.fallDelay = 400
                        obj.stayDelay = 5000
                        obj.blockColor = z
                        obj.scene = scene
                        scene.add(obj)
                        obj.play()
                    }
                }
            }
        }
    }
}
