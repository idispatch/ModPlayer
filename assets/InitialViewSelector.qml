import bb.cascades 1.3
import player 1.0

DropDown {
    title: qsTr("Start View") + Retranslate.onLanguageChanged
    Option {
        text: qsTr("Search") + Retranslate.onLanguageChanged
        value: "searchTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("My Favourite") + Retranslate.onLanguageChanged
        value: "myFavouriteTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Local Songs") + Retranslate.onLanguageChanged
        value: "localSongsTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Recent") + Retranslate.onLanguageChanged
        value: "recentlyPlayedTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Playlists") + Retranslate.onLanguageChanged
        value: "playlistsTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Albums") + Retranslate.onLanguageChanged
        value: "albumsTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("File System") + Retranslate.onLanguageChanged
        value: "filesTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Internet Radio") + Retranslate.onLanguageChanged
        value: "liveStreamTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Digitally Imported") + Retranslate.onLanguageChanged
        value: "difmTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("SKY.FM") + Retranslate.onLanguageChanged
        value: "skyfmTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Jazz Radio") + Retranslate.onLanguageChanged
        value: "jazzRadioTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Rock Radio") + Retranslate.onLanguageChanged
        value: "rockRadioTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Fresca Radio") + Retranslate.onLanguageChanged
        value: "frescaFmTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Most Played") + Retranslate.onLanguageChanged
        value: "mostPlayedTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Downloads") + Retranslate.onLanguageChanged
        value: "topDownloadsTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Favourited") + Retranslate.onLanguageChanged
        value: "topFavouritedTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Scored") + Retranslate.onLanguageChanged
        value: "topScoredTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Artists") + Retranslate.onLanguageChanged
        value: "artistsTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Genres") + Retranslate.onLanguageChanged
        value: "genresTab"
        selected: app.initialView == value 
    }
    Option {
        text: qsTr("Formats") + Retranslate.onLanguageChanged
        value: "formatsTab"
        selected: app.initialView == value 
    }
    onSelectedValueChanged: {
        app.initialView = selectedValue 
    }
}