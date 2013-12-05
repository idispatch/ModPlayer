#include <bb/cascades/Application>
#include "applicationui.hpp"

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
#include "ModPlayback.hpp"
#include "PatternView.hpp"
#include "VUMeter.hpp"
#include "TouchHandler.hpp"
#include "ItemGroupBase.hpp"

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
    DUMP_COUNT(ModPlayback)
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
#undef DUMP_COUNT
}
#endif

Q_DECL_EXPORT int main(int argc, char **argv) {
    int rc;
    ::umask(0);
    QCoreApplication::setOrganizationName("Kosenkov");
    QCoreApplication::setOrganizationDomain("kosenkov.ca");
    QCoreApplication::setApplicationName("ModPlayer");
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
