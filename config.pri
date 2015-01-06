# Config.pri file version 2.0. Auto-generated by IDE. Any changes made by user will be lost!
BASEDIR = $$quote($$_PRO_FILE_PWD_)

device {
    CONFIG(debug, debug|release) {
        !profile {
            LIBS += -lFlurry

            LIBS += $$quote(-L$$BASEDIR/libs/armle-v7)

            PRE_TARGETDEPS += $$quote($$BASEDIR/libs/armle-v7)

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }

    CONFIG(release, debug|release) {
        !profile {
            LIBS += -lFlurry

            LIBS += $$quote(-L$$BASEDIR/libs/armle-v7)

            PRE_TARGETDEPS += $$quote($$BASEDIR/libs/armle-v7)

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

simulator {
    CONFIG(debug, debug|release) {
        !profile {
            LIBS += -lFlurry

            LIBS += $$quote(-L$$BASEDIR/libs/x86)

            PRE_TARGETDEPS += $$quote($$BASEDIR/libs/x86)

            CONFIG += \
                config_pri_assets \
                config_pri_source_group1
        }
    }
}

config_pri_assets {
    OTHER_FILES += \
        $$quote($$BASEDIR/assets/AddFavouriteActionItem.qml) \
        $$quote($$BASEDIR/assets/Albums.qml) \
        $$quote($$BASEDIR/assets/Animation.qml) \
        $$quote($$BASEDIR/assets/AppWorldActionItem.qml) \
        $$quote($$BASEDIR/assets/AppendPlaylistActionItem.qml) \
        $$quote($$BASEDIR/assets/ArtistsList.qml) \
        $$quote($$BASEDIR/assets/BlackLabel.qml) \
        $$quote($$BASEDIR/assets/Buy.qml) \
        $$quote($$BASEDIR/assets/BuyActionItem.qml) \
        $$quote($$BASEDIR/assets/ColorListItem.qml) \
        $$quote($$BASEDIR/assets/Cover.qml) \
        $$quote($$BASEDIR/assets/CreatePlaylistActionItem.qml) \
        $$quote($$BASEDIR/assets/DeleteAllPlaylistsActionItem.qml) \
        $$quote($$BASEDIR/assets/ExportCacheButton.qml) \
        $$quote($$BASEDIR/assets/FirstLaunchHint.qml) \
        $$quote($$BASEDIR/assets/FormatsList.qml) \
        $$quote($$BASEDIR/assets/GenresList.qml) \
        $$quote($$BASEDIR/assets/GroupContainer.qml) \
        $$quote($$BASEDIR/assets/Help.qml) \
        $$quote($$BASEDIR/assets/HorizontalContainer.qml) \
        $$quote($$BASEDIR/assets/ImportButton.qml) \
        $$quote($$BASEDIR/assets/ImportSongsActionItem.qml) \
        $$quote($$BASEDIR/assets/Instruments.qml) \
        $$quote($$BASEDIR/assets/InstrumentsActionItem.qml) \
        $$quote($$BASEDIR/assets/InstrumentsView.qml) \
        $$quote($$BASEDIR/assets/InternetRadioList.qml) \
        $$quote($$BASEDIR/assets/LiveStreamRadio.qml) \
        $$quote($$BASEDIR/assets/LockIcon.qml) \
        $$quote($$BASEDIR/assets/ModPlayerListItem.qml) \
        $$quote($$BASEDIR/assets/Mouse.qml) \
        $$quote($$BASEDIR/assets/NextActionItem.qml) \
        $$quote($$BASEDIR/assets/OpenSongActionItem.qml) \
        $$quote($$BASEDIR/assets/Patterns.qml) \
        $$quote($$BASEDIR/assets/PatternsActionItem.qml) \
        $$quote($$BASEDIR/assets/PlayAllActionItem.qml) \
        $$quote($$BASEDIR/assets/PlayModeActionItem.qml) \
        $$quote($$BASEDIR/assets/PlaybackSettings.qml) \
        $$quote($$BASEDIR/assets/PlayerActionItem.qml) \
        $$quote($$BASEDIR/assets/PlayerTitleBar.qml) \
        $$quote($$BASEDIR/assets/PlaylistControl.qml) \
        $$quote($$BASEDIR/assets/PlaylistNameEntryPrompt.qml) \
        $$quote($$BASEDIR/assets/Playlists.qml) \
        $$quote($$BASEDIR/assets/PleaseBuy.qml) \
        $$quote($$BASEDIR/assets/PlusFeature.qml) \
        $$quote($$BASEDIR/assets/PreviousActionItem.qml) \
        $$quote($$BASEDIR/assets/ProgressComponent.qml) \
        $$quote($$BASEDIR/assets/ProgressToast.qml) \
        $$quote($$BASEDIR/assets/RadioMiniBadge.qml) \
        $$quote($$BASEDIR/assets/RemoveFavouriteActionItem.qml) \
        $$quote($$BASEDIR/assets/SameAlbumActionItem.qml) \
        $$quote($$BASEDIR/assets/SameArtistActionItem.qml) \
        $$quote($$BASEDIR/assets/SaveSongActionItem.qml) \
        $$quote($$BASEDIR/assets/SearchArea.qml) \
        $$quote($$BASEDIR/assets/SelectColor.qml) \
        $$quote($$BASEDIR/assets/Settings.qml) \
        $$quote($$BASEDIR/assets/SleepTimerActionItem.qml) \
        $$quote($$BASEDIR/assets/SleepTimerSettings.qml) \
        $$quote($$BASEDIR/assets/SongAlbumArt.qml) \
        $$quote($$BASEDIR/assets/SongArtistIcon.qml) \
        $$quote($$BASEDIR/assets/SongArtistInfo.qml) \
        $$quote($$BASEDIR/assets/SongDynamicInfo.qml) \
        $$quote($$BASEDIR/assets/SongIconView.qml) \
        $$quote($$BASEDIR/assets/SongList.qml) \
        $$quote($$BASEDIR/assets/SongMainInfo.qml) \
        $$quote($$BASEDIR/assets/SongParametersInfo.qml) \
        $$quote($$BASEDIR/assets/SongPersonalInfo.qml) \
        $$quote($$BASEDIR/assets/SongPlayer.qml) \
        $$quote($$BASEDIR/assets/SongProgressView.qml) \
        $$quote($$BASEDIR/assets/SongPublicInfo.qml) \
        $$quote($$BASEDIR/assets/SongView.qml) \
        $$quote($$BASEDIR/assets/TitleBarText.qml) \
        $$quote($$BASEDIR/assets/VerticalContainer.qml) \
        $$quote($$BASEDIR/assets/WallpaperSelector.qml) \
        $$quote($$BASEDIR/assets/WallpaperView.qml) \
        $$quote($$BASEDIR/assets/catalog.sqlite) \
        $$quote($$BASEDIR/assets/colors.json) \
        $$quote($$BASEDIR/assets/difm.json) \
        $$quote($$BASEDIR/assets/functions.js) \
        $$quote($$BASEDIR/assets/images/actions/icon_albums.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_append_playlist.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_artist.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_artists.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_bbm.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_buy.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_buy_now.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_delete.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_difm.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_downloads.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_email.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_favourite.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_format.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_genres.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_import.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_jazzradio.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_like.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_load_song.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_lock.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_mostplayed.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_my_songs.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_myfavourite.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_next.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_note.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_nowplaying.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_patterns.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_pause.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_play.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_playlist.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_playlist_mode.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_playonce.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_previous.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_radio.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_recent.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_repeatsong.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_review.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_rockradio.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_samples.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_save_song.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_score.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_search.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_search_dark.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_set_playlist.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_skyfm.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_stop.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_twitter.png) \
        $$quote($$BASEDIR/assets/images/actions/icon_unlike.png) \
        $$quote($$BASEDIR/assets/images/animation/modplayer.gif) \
        $$quote($$BASEDIR/assets/images/backgrounds/container_back.amd) \
        $$quote($$BASEDIR/assets/images/backgrounds/container_back.png) \
        $$quote($$BASEDIR/assets/images/backgrounds/infoback.amd) \
        $$quote($$BASEDIR/assets/images/backgrounds/infoback.png) \
        $$quote($$BASEDIR/assets/images/backgrounds/playlist_counter.amd) \
        $$quote($$BASEDIR/assets/images/backgrounds/playlist_counter.png) \
        $$quote($$BASEDIR/assets/images/backgrounds/view_back.amd) \
        $$quote($$BASEDIR/assets/images/backgrounds/view_back.png) \
        $$quote($$BASEDIR/assets/images/backgrounds/vu_back.amd) \
        $$quote($$BASEDIR/assets/images/backgrounds/vu_back.png) \
        $$quote($$BASEDIR/assets/images/badges/badge_downloads.png) \
        $$quote($$BASEDIR/assets/images/badges/badge_favourite.png) \
        $$quote($$BASEDIR/assets/images/badges/badge_lastplayed.png) \
        $$quote($$BASEDIR/assets/images/badges/badge_myfavourite.png) \
        $$quote($$BASEDIR/assets/images/badges/badge_played.png) \
        $$quote($$BASEDIR/assets/images/badges/badge_score.png) \
        $$quote($$BASEDIR/assets/images/cover/cover_text_back.amd) \
        $$quote($$BASEDIR/assets/images/cover/cover_text_back.png) \
        $$quote($$BASEDIR/assets/images/flags/Andorra-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Armenia-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Austria-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Azerbaijan-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Belarus-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Belgium-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Bosnian-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Bulgaria-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Croatian-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Cyprus-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Czech-republic.png) \
        $$quote($$BASEDIR/assets/images/flags/Denmark.png) \
        $$quote($$BASEDIR/assets/images/flags/Estonia.png) \
        $$quote($$BASEDIR/assets/images/flags/Faroe-islands.png) \
        $$quote($$BASEDIR/assets/images/flags/Finland-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/France-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Georgia-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Germany-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Gibraltar-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Greece-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Hungary-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Iceland-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Ireland-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Italy-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Kosovo.png) \
        $$quote($$BASEDIR/assets/images/flags/Latvia-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Liechtenstein-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Lithuania-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Luxembourg-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Macedonia-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Malta-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Moldova-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Monaco-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Montenegro.png) \
        $$quote($$BASEDIR/assets/images/flags/Netherlands-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Norway-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Poland-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Portugal-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Romania-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Russia-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Serbia-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Slovakia-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Slovenia-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Spain-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Sweden-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Switzerland-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Turkey-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/Ukraine-flag.png) \
        $$quote($$BASEDIR/assets/images/flags/United-kingdom-flag.png) \
        $$quote($$BASEDIR/assets/images/formats/icon-difm.png) \
        $$quote($$BASEDIR/assets/images/formats/icon-jazzradio.png) \
        $$quote($$BASEDIR/assets/images/formats/icon-rockradio.png) \
        $$quote($$BASEDIR/assets/images/formats/icon-skyfm.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_669.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_album.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_it.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_med.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_mod.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_mp3.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_mtm.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_oct.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_okt.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_playlist.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_s3m.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_stm.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_unknown.png) \
        $$quote($$BASEDIR/assets/images/formats/icon_xm.png) \
        $$quote($$BASEDIR/assets/images/indicators/11khz.png) \
        $$quote($$BASEDIR/assets/images/indicators/16bit.png) \
        $$quote($$BASEDIR/assets/images/indicators/22khz.png) \
        $$quote($$BASEDIR/assets/images/indicators/32bit.png) \
        $$quote($$BASEDIR/assets/images/indicators/44khz.png) \
        $$quote($$BASEDIR/assets/images/indicators/8bit.png) \
        $$quote($$BASEDIR/assets/images/indicators/fir.png) \
        $$quote($$BASEDIR/assets/images/indicators/highquality.png) \
        $$quote($$BASEDIR/assets/images/indicators/linear.png) \
        $$quote($$BASEDIR/assets/images/indicators/mono.png) \
        $$quote($$BASEDIR/assets/images/indicators/nearest.png) \
        $$quote($$BASEDIR/assets/images/indicators/reverb.png) \
        $$quote($$BASEDIR/assets/images/indicators/spline.png) \
        $$quote($$BASEDIR/assets/images/indicators/stereo.png) \
        $$quote($$BASEDIR/assets/images/indicators/surround.png) \
        $$quote($$BASEDIR/assets/images/indicators/xbass.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-0.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-1.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-10.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-2.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-3.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-4.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-5.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-6.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-7.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-8.png) \
        $$quote($$BASEDIR/assets/images/lcd/lcd-9.png) \
        $$quote($$BASEDIR/assets/images/led/led_off.png) \
        $$quote($$BASEDIR/assets/images/led/led_on.png) \
        $$quote($$BASEDIR/assets/images/objects/cache.png) \
        $$quote($$BASEDIR/assets/images/objects/lock.png) \
        $$quote($$BASEDIR/assets/images/objects/memory.png) \
        $$quote($$BASEDIR/assets/images/objects/mouse.png) \
        $$quote($$BASEDIR/assets/images/objects/pulldown.png) \
        $$quote($$BASEDIR/assets/images/objects/wallpaper.png) \
        $$quote($$BASEDIR/assets/images/vu/vu-off.png) \
        $$quote($$BASEDIR/assets/images/vu/vu-on.png) \
        $$quote($$BASEDIR/assets/images/wallpapers/abstract.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/aluminium.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/aluminium.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/asphalt.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/asphalt.png) \
        $$quote($$BASEDIR/assets/images/wallpapers/aurora.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/blue.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/blueprint.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/blueprint.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/carbon.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/chroma.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/clouds.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/cork.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/cork.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/crystals.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/dark-tile.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/dark-tile.png) \
        $$quote($$BASEDIR/assets/images/wallpapers/dawn.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/digital.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/dolphins.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/drops.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/flower.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/grass.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/green.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/green.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/grid.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/infinity.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/jeans.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/jeans.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/linen.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/linen.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/modplayer.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/modplayer.png) \
        $$quote($$BASEDIR/assets/images/wallpapers/plasma.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/purple.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/rainbow.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/rock.amd) \
        $$quote($$BASEDIR/assets/images/wallpapers/rock.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/sound.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/sparkle.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/stars.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/techno.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/water.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/waves.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/wood.jpg) \
        $$quote($$BASEDIR/assets/images/wallpapers/yellow.jpg) \
        $$quote($$BASEDIR/assets/jazzradio.json) \
        $$quote($$BASEDIR/assets/main.qml) \
        $$quote($$BASEDIR/assets/rockradio.json) \
        $$quote($$BASEDIR/assets/skyfm.json)
}

config_pri_source_group1 {
    SOURCES += \
        $$quote($$BASEDIR/src/Album.cpp) \
        $$quote($$BASEDIR/src/AlbumArtView.cpp) \
        $$quote($$BASEDIR/src/Analytics.cpp) \
        $$quote($$BASEDIR/src/ApplicationUI.cpp) \
        $$quote($$BASEDIR/src/Artist.cpp) \
        $$quote($$BASEDIR/src/Cache.cpp) \
        $$quote($$BASEDIR/src/Canvas.cpp) \
        $$quote($$BASEDIR/src/Catalog.cpp) \
        $$quote($$BASEDIR/src/CatalogAsync.cpp) \
        $$quote($$BASEDIR/src/CatalogInit.cpp) \
        $$quote($$BASEDIR/src/Downloader.cpp) \
        $$quote($$BASEDIR/src/FileSelector.cpp) \
        $$quote($$BASEDIR/src/FileUtils.cpp) \
        $$quote($$BASEDIR/src/Importer.cpp) \
        $$quote($$BASEDIR/src/InternetRadio.cpp) \
        $$quote($$BASEDIR/src/ItemGroupBase.cpp) \
        $$quote($$BASEDIR/src/LCDDigits.cpp) \
        $$quote($$BASEDIR/src/LCDDisplay.cpp) \
        $$quote($$BASEDIR/src/Mp3Export.cpp) \
        $$quote($$BASEDIR/src/NamedItem.cpp) \
        $$quote($$BASEDIR/src/NamedPlaylist.cpp) \
        $$quote($$BASEDIR/src/PatternView.cpp) \
        $$quote($$BASEDIR/src/PauseActionItem.cpp) \
        $$quote($$BASEDIR/src/PlayActionItem.cpp) \
        $$quote($$BASEDIR/src/Playback.cpp) \
        $$quote($$BASEDIR/src/PlaybackConfig.cpp) \
        $$quote($$BASEDIR/src/Player.cpp) \
        $$quote($$BASEDIR/src/Playlist.cpp) \
        $$quote($$BASEDIR/src/PurchaseStore.cpp) \
        $$quote($$BASEDIR/src/Radio.cpp) \
        $$quote($$BASEDIR/src/SongBasicInfo.cpp) \
        $$quote($$BASEDIR/src/SongExtendedInfo.cpp) \
        $$quote($$BASEDIR/src/SongFormat.cpp) \
        $$quote($$BASEDIR/src/SongGenre.cpp) \
        $$quote($$BASEDIR/src/SongModule.cpp) \
        $$quote($$BASEDIR/src/SuspendPlayback.cpp) \
        $$quote($$BASEDIR/src/TouchHandler.cpp) \
        $$quote($$BASEDIR/src/Unpacker.cpp) \
        $$quote($$BASEDIR/src/VUMeter.cpp) \
        $$quote($$BASEDIR/src/Wallpaper.cpp) \
        $$quote($$BASEDIR/src/WebImageView.cpp) \
        $$quote($$BASEDIR/src/lcd_font_6x8.cpp) \
        $$quote($$BASEDIR/src/libid3tag/id3_compat.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_crc.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_debug.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_field.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_file.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_frame.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_frametype.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_genre.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_latin1.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_parse.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_render.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_tag.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_ucs4.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_utf16.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_utf8.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_util.c) \
        $$quote($$BASEDIR/src/libid3tag/id3_version.c) \
        $$quote($$BASEDIR/src/libmad/mad_bit.c) \
        $$quote($$BASEDIR/src/libmad/mad_decoder.c) \
        $$quote($$BASEDIR/src/libmad/mad_fixed.c) \
        $$quote($$BASEDIR/src/libmad/mad_frame.c) \
        $$quote($$BASEDIR/src/libmad/mad_huffman.c) \
        $$quote($$BASEDIR/src/libmad/mad_layer12.c) \
        $$quote($$BASEDIR/src/libmad/mad_layer3.c) \
        $$quote($$BASEDIR/src/libmad/mad_minimad.c) \
        $$quote($$BASEDIR/src/libmad/mad_stream.c) \
        $$quote($$BASEDIR/src/libmad/mad_synth.c) \
        $$quote($$BASEDIR/src/libmad/mad_timer.c) \
        $$quote($$BASEDIR/src/libmad/mad_version.c) \
        $$quote($$BASEDIR/src/main.cpp) \
        $$quote($$BASEDIR/src/modplug/fastmix.cpp) \
        $$quote($$BASEDIR/src/modplug/load_669.cpp) \
        $$quote($$BASEDIR/src/modplug/load_abc.cpp) \
        $$quote($$BASEDIR/src/modplug/load_amf.cpp) \
        $$quote($$BASEDIR/src/modplug/load_ams.cpp) \
        $$quote($$BASEDIR/src/modplug/load_dbm.cpp) \
        $$quote($$BASEDIR/src/modplug/load_dmf.cpp) \
        $$quote($$BASEDIR/src/modplug/load_dsm.cpp) \
        $$quote($$BASEDIR/src/modplug/load_far.cpp) \
        $$quote($$BASEDIR/src/modplug/load_it.cpp) \
        $$quote($$BASEDIR/src/modplug/load_j2b.cpp) \
        $$quote($$BASEDIR/src/modplug/load_mdl.cpp) \
        $$quote($$BASEDIR/src/modplug/load_med.cpp) \
        $$quote($$BASEDIR/src/modplug/load_mid.cpp) \
        $$quote($$BASEDIR/src/modplug/load_mod.cpp) \
        $$quote($$BASEDIR/src/modplug/load_mt2.cpp) \
        $$quote($$BASEDIR/src/modplug/load_mtm.cpp) \
        $$quote($$BASEDIR/src/modplug/load_okt.cpp) \
        $$quote($$BASEDIR/src/modplug/load_pat.cpp) \
        $$quote($$BASEDIR/src/modplug/load_psm.cpp) \
        $$quote($$BASEDIR/src/modplug/load_ptm.cpp) \
        $$quote($$BASEDIR/src/modplug/load_s3m.cpp) \
        $$quote($$BASEDIR/src/modplug/load_stm.cpp) \
        $$quote($$BASEDIR/src/modplug/load_ult.cpp) \
        $$quote($$BASEDIR/src/modplug/load_umx.cpp) \
        $$quote($$BASEDIR/src/modplug/load_wav.cpp) \
        $$quote($$BASEDIR/src/modplug/load_xm.cpp) \
        $$quote($$BASEDIR/src/modplug/mmcmp.cpp) \
        $$quote($$BASEDIR/src/modplug/modplug.cpp) \
        $$quote($$BASEDIR/src/modplug/snd_dsp.cpp) \
        $$quote($$BASEDIR/src/modplug/snd_flt.cpp) \
        $$quote($$BASEDIR/src/modplug/snd_fx.cpp) \
        $$quote($$BASEDIR/src/modplug/sndfile.cpp) \
        $$quote($$BASEDIR/src/modplug/sndmix.cpp) \
        $$quote($$BASEDIR/src/mp3lame/VbrTag.c) \
        $$quote($$BASEDIR/src/mp3lame/bitstream.c) \
        $$quote($$BASEDIR/src/mp3lame/encoder.c) \
        $$quote($$BASEDIR/src/mp3lame/fft.c) \
        $$quote($$BASEDIR/src/mp3lame/gain_analysis.c) \
        $$quote($$BASEDIR/src/mp3lame/id3tag.c) \
        $$quote($$BASEDIR/src/mp3lame/lame.c) \
        $$quote($$BASEDIR/src/mp3lame/mpglib_interface.c) \
        $$quote($$BASEDIR/src/mp3lame/newmdct.c) \
        $$quote($$BASEDIR/src/mp3lame/presets.c) \
        $$quote($$BASEDIR/src/mp3lame/psymodel.c) \
        $$quote($$BASEDIR/src/mp3lame/quantize.c) \
        $$quote($$BASEDIR/src/mp3lame/quantize_pvt.c) \
        $$quote($$BASEDIR/src/mp3lame/reservoir.c) \
        $$quote($$BASEDIR/src/mp3lame/set_get.c) \
        $$quote($$BASEDIR/src/mp3lame/tables.c) \
        $$quote($$BASEDIR/src/mp3lame/takehiro.c) \
        $$quote($$BASEDIR/src/mp3lame/util.c) \
        $$quote($$BASEDIR/src/mp3lame/vbrquantize.c) \
        $$quote($$BASEDIR/src/mp3lame/vector/xmm_quantize_sub.c) \
        $$quote($$BASEDIR/src/mp3lame/version.c) \
        $$quote($$BASEDIR/src/zip/JlCompress.cpp) \
        $$quote($$BASEDIR/src/zip/qioapi.cpp) \
        $$quote($$BASEDIR/src/zip/quaadler32.cpp) \
        $$quote($$BASEDIR/src/zip/quacrc32.cpp) \
        $$quote($$BASEDIR/src/zip/quagzipfile.cpp) \
        $$quote($$BASEDIR/src/zip/quaziodevice.cpp) \
        $$quote($$BASEDIR/src/zip/quazip.cpp) \
        $$quote($$BASEDIR/src/zip/quazipdir.cpp) \
        $$quote($$BASEDIR/src/zip/quazipfile.cpp) \
        $$quote($$BASEDIR/src/zip/quazipfileinfo.cpp) \
        $$quote($$BASEDIR/src/zip/quazipnewinfo.cpp) \
        $$quote($$BASEDIR/src/zip/unzip.c) \
        $$quote($$BASEDIR/src/zip/zip.c)

    HEADERS += \
        $$quote($$BASEDIR/src/Album.hpp) \
        $$quote($$BASEDIR/src/AlbumArtView.hpp) \
        $$quote($$BASEDIR/src/Analytics.hpp) \
        $$quote($$BASEDIR/src/ApplicationUI.hpp) \
        $$quote($$BASEDIR/src/Artist.hpp) \
        $$quote($$BASEDIR/src/Cache.hpp) \
        $$quote($$BASEDIR/src/Canvas.hpp) \
        $$quote($$BASEDIR/src/Catalog.hpp) \
        $$quote($$BASEDIR/src/CatalogMigration.hpp) \
        $$quote($$BASEDIR/src/Downloader.hpp) \
        $$quote($$BASEDIR/src/FileSelector.hpp) \
        $$quote($$BASEDIR/src/FileUtils.hpp) \
        $$quote($$BASEDIR/src/Flurry/Flurry.h) \
        $$quote($$BASEDIR/src/Importer.hpp) \
        $$quote($$BASEDIR/src/InstanceCounter.hpp) \
        $$quote($$BASEDIR/src/InternetRadio.hpp) \
        $$quote($$BASEDIR/src/ItemGroupBase.hpp) \
        $$quote($$BASEDIR/src/LCDDigits.hpp) \
        $$quote($$BASEDIR/src/LCDDisplay.hpp) \
        $$quote($$BASEDIR/src/MessageBox.hpp) \
        $$quote($$BASEDIR/src/Mp3Export.hpp) \
        $$quote($$BASEDIR/src/NamedItem.hpp) \
        $$quote($$BASEDIR/src/NamedPlaylist.hpp) \
        $$quote($$BASEDIR/src/PatternView.hpp) \
        $$quote($$BASEDIR/src/PauseActionItem.hpp) \
        $$quote($$BASEDIR/src/PlayActionItem.hpp) \
        $$quote($$BASEDIR/src/Playback.hpp) \
        $$quote($$BASEDIR/src/PlaybackConfig.hpp) \
        $$quote($$BASEDIR/src/Player.hpp) \
        $$quote($$BASEDIR/src/Playlist.hpp) \
        $$quote($$BASEDIR/src/PurchaseStore.hpp) \
        $$quote($$BASEDIR/src/Radio.hpp) \
        $$quote($$BASEDIR/src/SongBasicInfo.hpp) \
        $$quote($$BASEDIR/src/SongExtendedInfo.hpp) \
        $$quote($$BASEDIR/src/SongFormat.hpp) \
        $$quote($$BASEDIR/src/SongGenre.hpp) \
        $$quote($$BASEDIR/src/SongModule.hpp) \
        $$quote($$BASEDIR/src/SqlReader.hpp) \
        $$quote($$BASEDIR/src/SuspendPlayback.hpp) \
        $$quote($$BASEDIR/src/TouchHandler.hpp) \
        $$quote($$BASEDIR/src/Unpacker.hpp) \
        $$quote($$BASEDIR/src/VUMeter.hpp) \
        $$quote($$BASEDIR/src/Wallpaper.hpp) \
        $$quote($$BASEDIR/src/WebImageView.hpp) \
        $$quote($$BASEDIR/src/lcd_fonts.hpp) \
        $$quote($$BASEDIR/src/libid3tag/id3_compat.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_crc.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_debug.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_field.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_file.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_frame.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_frametype.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_genre.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_global.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_id3tag.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_latin1.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_parse.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_render.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_tag.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_ucs4.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_utf16.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_utf8.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_util.h) \
        $$quote($$BASEDIR/src/libid3tag/id3_version.h) \
        $$quote($$BASEDIR/src/libid3tag/msvc++/config.h) \
        $$quote($$BASEDIR/src/libmad/mad.h) \
        $$quote($$BASEDIR/src/libmad/mad_bit.h) \
        $$quote($$BASEDIR/src/libmad/mad_decoder.h) \
        $$quote($$BASEDIR/src/libmad/mad_fixed.h) \
        $$quote($$BASEDIR/src/libmad/mad_frame.h) \
        $$quote($$BASEDIR/src/libmad/mad_global.h) \
        $$quote($$BASEDIR/src/libmad/mad_huffman.h) \
        $$quote($$BASEDIR/src/libmad/mad_layer12.h) \
        $$quote($$BASEDIR/src/libmad/mad_layer3.h) \
        $$quote($$BASEDIR/src/libmad/mad_stream.h) \
        $$quote($$BASEDIR/src/libmad/mad_synth.h) \
        $$quote($$BASEDIR/src/libmad/mad_timer.h) \
        $$quote($$BASEDIR/src/libmad/mad_version.h) \
        $$quote($$BASEDIR/src/libmad/msvc++/config.h) \
        $$quote($$BASEDIR/src/libmad/msvc++/mad.h) \
        $$quote($$BASEDIR/src/modplug/config.h) \
        $$quote($$BASEDIR/src/modplug/it_defs.h) \
        $$quote($$BASEDIR/src/modplug/load_pat.h) \
        $$quote($$BASEDIR/src/modplug/modplug.h) \
        $$quote($$BASEDIR/src/modplug/sndfile.h) \
        $$quote($$BASEDIR/src/modplug/stdafx.h) \
        $$quote($$BASEDIR/src/modplug/tables.h) \
        $$quote($$BASEDIR/src/mp3lame/VbrTag.h) \
        $$quote($$BASEDIR/src/mp3lame/bitstream.h) \
        $$quote($$BASEDIR/src/mp3lame/encoder.h) \
        $$quote($$BASEDIR/src/mp3lame/fft.h) \
        $$quote($$BASEDIR/src/mp3lame/gain_analysis.h) \
        $$quote($$BASEDIR/src/mp3lame/id3tag.h) \
        $$quote($$BASEDIR/src/mp3lame/l3side.h) \
        $$quote($$BASEDIR/src/mp3lame/lame-analysis.h) \
        $$quote($$BASEDIR/src/mp3lame/lame.h) \
        $$quote($$BASEDIR/src/mp3lame/lame_global_flags.h) \
        $$quote($$BASEDIR/src/mp3lame/lameerror.h) \
        $$quote($$BASEDIR/src/mp3lame/machine.h) \
        $$quote($$BASEDIR/src/mp3lame/newmdct.h) \
        $$quote($$BASEDIR/src/mp3lame/psymodel.h) \
        $$quote($$BASEDIR/src/mp3lame/quantize.h) \
        $$quote($$BASEDIR/src/mp3lame/quantize_pvt.h) \
        $$quote($$BASEDIR/src/mp3lame/reservoir.h) \
        $$quote($$BASEDIR/src/mp3lame/set_get.h) \
        $$quote($$BASEDIR/src/mp3lame/tables.h) \
        $$quote($$BASEDIR/src/mp3lame/util.h) \
        $$quote($$BASEDIR/src/mp3lame/vbrquantize.h) \
        $$quote($$BASEDIR/src/mp3lame/vector/lame_intrin.h) \
        $$quote($$BASEDIR/src/mp3lame/version.h) \
        $$quote($$BASEDIR/src/zip/JlCompress.h) \
        $$quote($$BASEDIR/src/zip/crypt.h) \
        $$quote($$BASEDIR/src/zip/ioapi.h) \
        $$quote($$BASEDIR/src/zip/quaadler32.h) \
        $$quote($$BASEDIR/src/zip/quachecksum32.h) \
        $$quote($$BASEDIR/src/zip/quacrc32.h) \
        $$quote($$BASEDIR/src/zip/quagzipfile.h) \
        $$quote($$BASEDIR/src/zip/quaziodevice.h) \
        $$quote($$BASEDIR/src/zip/quazip.h) \
        $$quote($$BASEDIR/src/zip/quazip_global.h) \
        $$quote($$BASEDIR/src/zip/quazipdir.h) \
        $$quote($$BASEDIR/src/zip/quazipfile.h) \
        $$quote($$BASEDIR/src/zip/quazipfileinfo.h) \
        $$quote($$BASEDIR/src/zip/quazipnewinfo.h) \
        $$quote($$BASEDIR/src/zip/unzip.h) \
        $$quote($$BASEDIR/src/zip/zip.h)
}

INCLUDEPATH += $$quote($$BASEDIR/src/libid3tag) \
    $$quote($$BASEDIR/src/Flurry) \
    $$quote($$BASEDIR/src/zip) \
    $$quote($$BASEDIR/src/mp3lame/vector) \
    $$quote($$BASEDIR/src/mp3lame) \
    $$quote($$BASEDIR/src/libmad/msvc++) \
    $$quote($$BASEDIR/src/libmad) \
    $$quote($$BASEDIR/src/modplug) \
    $$quote($$BASEDIR/src/libid3tag/msvc++) \
    $$quote($$BASEDIR/src)

CONFIG += precompile_header

PRECOMPILED_HEADER = $$quote($$BASEDIR/precompiled.h)

lupdate_inclusion {
    SOURCES += \
        $$quote($$BASEDIR/../src/*.c) \
        $$quote($$BASEDIR/../src/*.c++) \
        $$quote($$BASEDIR/../src/*.cc) \
        $$quote($$BASEDIR/../src/*.cpp) \
        $$quote($$BASEDIR/../src/*.cxx) \
        $$quote($$BASEDIR/../src/Flurry/*.c) \
        $$quote($$BASEDIR/../src/Flurry/*.c++) \
        $$quote($$BASEDIR/../src/Flurry/*.cc) \
        $$quote($$BASEDIR/../src/Flurry/*.cpp) \
        $$quote($$BASEDIR/../src/Flurry/*.cxx) \
        $$quote($$BASEDIR/../src/libid3tag/*.c) \
        $$quote($$BASEDIR/../src/libid3tag/*.c++) \
        $$quote($$BASEDIR/../src/libid3tag/*.cc) \
        $$quote($$BASEDIR/../src/libid3tag/*.cpp) \
        $$quote($$BASEDIR/../src/libid3tag/*.cxx) \
        $$quote($$BASEDIR/../src/libid3tag/msvc++/*.c) \
        $$quote($$BASEDIR/../src/libid3tag/msvc++/*.c++) \
        $$quote($$BASEDIR/../src/libid3tag/msvc++/*.cc) \
        $$quote($$BASEDIR/../src/libid3tag/msvc++/*.cpp) \
        $$quote($$BASEDIR/../src/libid3tag/msvc++/*.cxx) \
        $$quote($$BASEDIR/../src/libmad/*.c) \
        $$quote($$BASEDIR/../src/libmad/*.c++) \
        $$quote($$BASEDIR/../src/libmad/*.cc) \
        $$quote($$BASEDIR/../src/libmad/*.cpp) \
        $$quote($$BASEDIR/../src/libmad/*.cxx) \
        $$quote($$BASEDIR/../src/libmad/msvc++/*.c) \
        $$quote($$BASEDIR/../src/libmad/msvc++/*.c++) \
        $$quote($$BASEDIR/../src/libmad/msvc++/*.cc) \
        $$quote($$BASEDIR/../src/libmad/msvc++/*.cpp) \
        $$quote($$BASEDIR/../src/libmad/msvc++/*.cxx) \
        $$quote($$BASEDIR/../src/modplug/*.c) \
        $$quote($$BASEDIR/../src/modplug/*.c++) \
        $$quote($$BASEDIR/../src/modplug/*.cc) \
        $$quote($$BASEDIR/../src/modplug/*.cpp) \
        $$quote($$BASEDIR/../src/modplug/*.cxx) \
        $$quote($$BASEDIR/../src/mp3lame/*.c) \
        $$quote($$BASEDIR/../src/mp3lame/*.c++) \
        $$quote($$BASEDIR/../src/mp3lame/*.cc) \
        $$quote($$BASEDIR/../src/mp3lame/*.cpp) \
        $$quote($$BASEDIR/../src/mp3lame/*.cxx) \
        $$quote($$BASEDIR/../src/mp3lame/vector/*.c) \
        $$quote($$BASEDIR/../src/mp3lame/vector/*.c++) \
        $$quote($$BASEDIR/../src/mp3lame/vector/*.cc) \
        $$quote($$BASEDIR/../src/mp3lame/vector/*.cpp) \
        $$quote($$BASEDIR/../src/mp3lame/vector/*.cxx) \
        $$quote($$BASEDIR/../src/zip/*.c) \
        $$quote($$BASEDIR/../src/zip/*.c++) \
        $$quote($$BASEDIR/../src/zip/*.cc) \
        $$quote($$BASEDIR/../src/zip/*.cpp) \
        $$quote($$BASEDIR/../src/zip/*.cxx) \
        $$quote($$BASEDIR/../assets/*.qml) \
        $$quote($$BASEDIR/../assets/*.js) \
        $$quote($$BASEDIR/../assets/*.qs) \
        $$quote($$BASEDIR/../assets/images/*.qml) \
        $$quote($$BASEDIR/../assets/images/*.js) \
        $$quote($$BASEDIR/../assets/images/*.qs) \
        $$quote($$BASEDIR/../assets/images/actions/*.qml) \
        $$quote($$BASEDIR/../assets/images/actions/*.js) \
        $$quote($$BASEDIR/../assets/images/actions/*.qs) \
        $$quote($$BASEDIR/../assets/images/animation/*.qml) \
        $$quote($$BASEDIR/../assets/images/animation/*.js) \
        $$quote($$BASEDIR/../assets/images/animation/*.qs) \
        $$quote($$BASEDIR/../assets/images/backgrounds/*.qml) \
        $$quote($$BASEDIR/../assets/images/backgrounds/*.js) \
        $$quote($$BASEDIR/../assets/images/backgrounds/*.qs) \
        $$quote($$BASEDIR/../assets/images/badges/*.qml) \
        $$quote($$BASEDIR/../assets/images/badges/*.js) \
        $$quote($$BASEDIR/../assets/images/badges/*.qs) \
        $$quote($$BASEDIR/../assets/images/cover/*.qml) \
        $$quote($$BASEDIR/../assets/images/cover/*.js) \
        $$quote($$BASEDIR/../assets/images/cover/*.qs) \
        $$quote($$BASEDIR/../assets/images/flags/*.qml) \
        $$quote($$BASEDIR/../assets/images/flags/*.js) \
        $$quote($$BASEDIR/../assets/images/flags/*.qs) \
        $$quote($$BASEDIR/../assets/images/formats/*.qml) \
        $$quote($$BASEDIR/../assets/images/formats/*.js) \
        $$quote($$BASEDIR/../assets/images/formats/*.qs) \
        $$quote($$BASEDIR/../assets/images/indicators/*.qml) \
        $$quote($$BASEDIR/../assets/images/indicators/*.js) \
        $$quote($$BASEDIR/../assets/images/indicators/*.qs) \
        $$quote($$BASEDIR/../assets/images/lcd/*.qml) \
        $$quote($$BASEDIR/../assets/images/lcd/*.js) \
        $$quote($$BASEDIR/../assets/images/lcd/*.qs) \
        $$quote($$BASEDIR/../assets/images/led/*.qml) \
        $$quote($$BASEDIR/../assets/images/led/*.js) \
        $$quote($$BASEDIR/../assets/images/led/*.qs) \
        $$quote($$BASEDIR/../assets/images/objects/*.qml) \
        $$quote($$BASEDIR/../assets/images/objects/*.js) \
        $$quote($$BASEDIR/../assets/images/objects/*.qs) \
        $$quote($$BASEDIR/../assets/images/vu/*.qml) \
        $$quote($$BASEDIR/../assets/images/vu/*.js) \
        $$quote($$BASEDIR/../assets/images/vu/*.qs) \
        $$quote($$BASEDIR/../assets/images/wallpapers/*.qml) \
        $$quote($$BASEDIR/../assets/images/wallpapers/*.js) \
        $$quote($$BASEDIR/../assets/images/wallpapers/*.qs)

    HEADERS += \
        $$quote($$BASEDIR/../src/*.h) \
        $$quote($$BASEDIR/../src/*.h++) \
        $$quote($$BASEDIR/../src/*.hh) \
        $$quote($$BASEDIR/../src/*.hpp) \
        $$quote($$BASEDIR/../src/*.hxx)
}

TRANSLATIONS = $$quote($${TARGET}_ru.ts) \
    $$quote($${TARGET}_zh.ts) \
    $$quote($${TARGET}.ts)
