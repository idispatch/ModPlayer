#include <bb/cascades/Application>
#include "applicationui.hpp"
#include <iostream>
#include <iomanip>

#define DUMP_INSTANCE_COUNTS 1

#if defined(DUMP_INSTANCE_COUNTS)
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
#include "VUMeter.hpp"
#include "ItemGroupBase.hpp"

static void dumpInstanceCounts() {
    std::cerr << "* Analytics objects:        " << Analytics::getInstanceCount() << ", max count: " << Analytics::getMaxInstanceCount() << std::endl;
    std::cerr << "* PlaybackConfig objects:   " << PlaybackConfig::getInstanceCount() << ", max count: " << PlaybackConfig::getMaxInstanceCount() << std::endl;
    std::cerr << "* Downloader objects:       " << Downloader::getInstanceCount() << ", max count: " << Downloader::getMaxInstanceCount() << std::endl;
    std::cerr << "* Unpacker objects:         " << Unpacker::getInstanceCount() << ", max count: " << Unpacker::getMaxInstanceCount() << std::endl;
    std::cerr << "* Catalog objects:          " << Catalog::getInstanceCount() << ", max count: " << Catalog::getMaxInstanceCount() << std::endl;
    std::cerr << "* Cache objects:            " << Cache::getInstanceCount() << ", max count: " << Cache::getMaxInstanceCount() << std::endl;
    std::cerr << "* Canvas objects:           " << Canvas::getInstanceCount() << ", max count: " << Canvas::getMaxInstanceCount() << std::endl;
    std::cerr << "* Player objects:           " << Player::getInstanceCount() << ", max count: " << Player::getMaxInstanceCount() << std::endl;
    std::cerr << "* ModPlayback objects:      " << ModPlayback::getInstanceCount() << ", max count: " << ModPlayback::getMaxInstanceCount() << std::endl;
    std::cerr << "* ItemGroupBase objects:    " << ItemGroupBase::getInstanceCount() << ", max count: " << ItemGroupBase::getMaxInstanceCount() << std::endl;
    std::cerr << "* LCDDigits objects:        " << LCDDigits::getInstanceCount() << ", max count: " << LCDDigits::getMaxInstanceCount() << std::endl;
    std::cerr << "* LCDDisplay objects:       " << LCDDisplay::getInstanceCount() << ", max count: " << LCDDisplay::getMaxInstanceCount() << std::endl;
    std::cerr << "* Artist objects:           " << Artist::getInstanceCount() << ", max count: " << Artist::getMaxInstanceCount() << std::endl;
    std::cerr << "* SongGenre objects:        " << SongGenre::getInstanceCount() << ", max count: " << SongGenre::getMaxInstanceCount() << std::endl;
    std::cerr << "* SongFormat objects:       " << SongFormat::getInstanceCount() << ", max count: " << SongFormat::getMaxInstanceCount() << std::endl;
    std::cerr << "* SongBasicInfo objects:    " << SongBasicInfo::getInstanceCount() << ", max count: " << SongBasicInfo::getMaxInstanceCount() << std::endl;
    std::cerr << "* SongExtendedInfo objects: " << SongExtendedInfo::getInstanceCount() << ", max count: " << SongExtendedInfo::getMaxInstanceCount() << std::endl;
    std::cerr << "* SongModule objects:       " << SongModule::getInstanceCount() << ", max count: " << SongModule::getMaxInstanceCount() << std::endl;
    std::cerr << "* VUMeter objects:          " << VUMeter::getInstanceCount() << ", max count: " << SongModule::getMaxInstanceCount() << std::endl;
}
#endif

Q_DECL_EXPORT int main(int argc, char **argv) {
    int rc;
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
