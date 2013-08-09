import bb.cascades 1.0
import player 1.0

Container {
    id: songPlayData
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    
    property real lcd_scale: 0.5
    property bool songLoaded: true

    property alias songOrder: lcdOrder.number
    property alias songPattern: lcdPattern.number
    property alias songRow: lcdRow.number
    property alias songChannels: lcdChannels.number
    property alias songVolume: lcdVolume.number
    property alias songSpeed: lcdSpeed.number
    property alias songTempo: lcdTempo.number
    
    visible: songLoaded
    horizontalAlignment: HorizontalAlignment.Fill
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }
        rightMargin: 40
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Order"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdOrder
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
                scaleX: songPlayData.lcd_scale
                scaleY: songPlayData.lcd_scale
            }
        }
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Pattern"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdPattern
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
                scaleX: songPlayData.lcd_scale
                scaleY: songPlayData.lcd_scale
            }
        }
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Row"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdRow
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
                scaleX: songPlayData.lcd_scale
                scaleY: songPlayData.lcd_scale
            }
        }
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Channels"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdChannels
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
                scaleX: songPlayData.lcd_scale
                scaleY: songPlayData.lcd_scale
            }
        }
    }
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }
        leftMargin: 40
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Speed"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdSpeed
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
                scaleX: songPlayData.lcd_scale
                scaleY: songPlayData.lcd_scale
            }
        }
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Tempo"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdTempo
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
                scaleX: songPlayData.lcd_scale
                scaleY: songPlayData.lcd_scale
            }
        }
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Volume"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdVolume
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
                scaleX: songPlayData.lcd_scale
                scaleY: songPlayData.lcd_scale
            }
        }
    }
}