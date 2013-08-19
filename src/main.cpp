#include <bb/cascades/Application>
#include "applicationui.hpp"
#include <iostream>
#include <iomanip>

#define DUMP_INSTANCE_COUNTS 1

#if defined(DUMP_INSTANCE_COUNTS)
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
#include "ItemGroupBase.hpp"

static void dumpInstanceCounts() {
    std::cerr << "* PlaybackConfig objects:   " << PlaybackConfig::getInstanceCount() << std::endl;
    std::cerr << "* Downloader objects:       " << Downloader::getInstanceCount() << std::endl;
    std::cerr << "* Unpacker objects:         " << Unpacker::getInstanceCount() << std::endl;
    std::cerr << "* Catalog objects:          " << Catalog::getInstanceCount() << std::endl;
    std::cerr << "* Cache objects:            " << Cache::getInstanceCount() << std::endl;
    std::cerr << "* Canvas objects:           " << Canvas::getInstanceCount() << std::endl;
    std::cerr << "* Player objects:           " << Player::getInstanceCount() << std::endl;
    std::cerr << "* ModPlayback objects:      " << ModPlayback::getInstanceCount() << std::endl;
    std::cerr << "* ItemGroupBase objects:    " << ItemGroupBase::getInstanceCount() << std::endl;
    std::cerr << "* LCDDigits objects:        " << LCDDigits::getInstanceCount() << std::endl;
    std::cerr << "* LCDDisplay objects:       " << LCDDisplay::getInstanceCount() << std::endl;
    std::cerr << "* Artist objects:           " << Artist::getInstanceCount() << std::endl;
    std::cerr << "* SongGenre objects:        " << SongGenre::getInstanceCount() << std::endl;
    std::cerr << "* SongBasicInfo objects:    " << SongBasicInfo::getInstanceCount() << std::endl;
    std::cerr << "* SongExtendedInfo objects: " << SongExtendedInfo::getInstanceCount() << std::endl;
    std::cerr << "* SongModule objects:       " << SongModule::getInstanceCount() << std::endl;
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
