import bb.cascades 1.0
import player 1.0

Container {
    id: songStatData
    
    layout: StackLayout {
        orientation: LayoutOrientation.LeftToRight
    }
    
    property bool songLoaded: true
    
    property alias songOrders: lcdOrders.number
    property alias songPatterns: lcdPatterns.number
    property alias songChannels: lcdChannels.number
    
    property alias songInstruments: lcdInstruments.number
    property alias songSamples: lcdSamples.number
    
    visible: songLoaded
    horizontalAlignment: HorizontalAlignment.Fill
    
    Container {
        layout: StackLayout {
            orientation: LayoutOrientation.TopToBottom
        }
        layoutProperties: StackLayoutProperties {
            spaceQuota: 1.0
        }
        rightMargin: 50
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Orders"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdOrders
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Patterns"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdPatterns
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
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
        leftMargin: 50
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Instruments"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdInstruments
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
        Container {
            layout: DockLayout {
            }
            horizontalAlignment: HorizontalAlignment.Fill
            Label {
                text: "Samples"
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Left
            }
            LCDDigits {
                id: lcdSamples
                verticalAlignment: VerticalAlignment.Center
                horizontalAlignment: HorizontalAlignment.Right
                numDigits: 3
            }
        }
    }
}