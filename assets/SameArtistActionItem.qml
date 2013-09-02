import bb.cascades 1.0

ActionItem {
    property variant currentSong
    property variant navigationPane
    
    title: qsTr("Songs by Artist")
    imageSource: "asset:///images/actions/icon_artist.png"
    enabled: currentSong && currentSong.artistId != 0 
    ActionBar.placement: ActionBarPlacement.InOverflow
    onTriggered: {
        
    }
}