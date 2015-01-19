#include <bb/ApplicationInfo>
#include <bb/cascades/Application>
#include "ApplicationUI.hpp"

#define DUMP_INSTANCE_COUNTS 1

#if defined(DUMP_INSTANCE_COUNTS)
#include <iostream>
#include <iomanip>

#include "Analytics.hpp"
#include "Artist.hpp"
#include "SongGenre.hpp"
#include "SongFormat.hpp"
#include "SongBasicInfo.hpp"
#include "SongExtendedInfo.hpp"
#include "SongModule.hpp"
#include "LCDDigits.hpp"
#include "LCDDisplay.hpp"
#include "Unpacker.hpp"
#include "Downloader.hpp"
#include "Catalog.hpp"
#include "Canvas.hpp"
#include "Cache.hpp"
#include "Player.hpp"
#include "PlaybackConfig.hpp"
#include "Playback.hpp"
#include "PatternView.hpp"
#include "VUMeter.hpp"
#include "TouchHandler.hpp"
#include "NamedItem.hpp"
#include "ItemGroupBase.hpp"
#include "Playlist.hpp"
#include "NamedPlaylist.hpp"
#include "Album.hpp"
#include "Radio.hpp"
#include "WebImageView.hpp"
#include "AlbumArtView.hpp"
#include "InternetRadio.hpp"
#include "Wallpaper.hpp"
#include "FileEntry.hpp"

template<typename T>
void dumpInstanceCount(const char * className) {
    std::cerr << "* " << className << " objects: "
              << T::getInstanceCount()
              << ", max count: "
              << T::getMaxInstanceCount()
              << std::endl;
}

static void dumpInstanceCounts() {
#define DUMP_COUNT(T) dumpInstanceCount<T>(#T);
    DUMP_COUNT(Analytics)
    DUMP_COUNT(PlaybackConfig)
    DUMP_COUNT(Downloader)
    DUMP_COUNT(Unpacker)
    DUMP_COUNT(Catalog)
    DUMP_COUNT(Cache)
    DUMP_COUNT(Cache)
    DUMP_COUNT(Canvas)
    DUMP_COUNT(Player)
    DUMP_COUNT(Playback)
    DUMP_COUNT(NamedItem)
    DUMP_COUNT(ItemGroupBase)
    DUMP_COUNT(LCDDigits)
    DUMP_COUNT(LCDDisplay)
    DUMP_COUNT(Artist)
    DUMP_COUNT(SongGenre)
    DUMP_COUNT(SongFormat)
    DUMP_COUNT(SongBasicInfo)
    DUMP_COUNT(SongExtendedInfo)
    DUMP_COUNT(SongModule)
    DUMP_COUNT(PatternView)
    DUMP_COUNT(VUMeter)
    DUMP_COUNT(TouchHandler)
    DUMP_COUNT(Playlist)
    DUMP_COUNT(NamedPlaylist)
    DUMP_COUNT(Album)
    DUMP_COUNT(Radio)
    DUMP_COUNT(WebImageView)
    DUMP_COUNT(AlbumArtView)
    DUMP_COUNT(InternetRadio)
    DUMP_COUNT(Wallpaper)
    DUMP_COUNT(FileEntry)
#undef DUMP_COUNT
}
#endif

Q_DECL_EXPORT int main(int argc, char **argv) {
    int rc;
    ::umask(0);
    QCoreApplication::setOrganizationName("Kosenkov");
    QCoreApplication::setOrganizationDomain("kosenkov.ca");
    QCoreApplication::setApplicationName(bb::ApplicationInfo().title());
    {
        bb::cascades::Application app(argc, argv);
        new ApplicationUI(&app);
        rc = bb::cascades::Application::exec();
    }
#if defined(DUMP_INSTANCE_COUNTS)
    dumpInstanceCounts();
#endif
    return rc;
}
