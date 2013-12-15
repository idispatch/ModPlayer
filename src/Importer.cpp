#include "modplug/modplug.h"
#include <dirent.h>
#include <fnmatch.h>
#include <errno.h>
#include "Importer.hpp"
#include "FileUtils.hpp"
#include "Analytics.hpp"
#include "SongExtendedInfo.hpp"
#include "SongFormat.hpp"
#include "Catalog.hpp"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>
#include <QStack>
#include <QByteArray>
#include <bb/system/SystemProgressDialog>

//#define DETAILED_LOG

using namespace bb::system;

Importer::Importer(QStringList const& filters,
                   Catalog * catalog,
                   QObject * parent)
    : QObject(parent),
      m_catalog(catalog),
      m_progress(NULL),
      m_filters(filters),
      m_numImportedSongs(0) {
}

Importer::~Importer() {
    destroyProgressUI();
}

int Importer::numImportedSongs() const {
    return m_numImportedSongs;
}

void Importer::clean() {
    m_catalog->clearPersonalSongs();
}

int Importer::import()
{
    m_numImportedSongs = 0;

    clean();
    createProgressUI();

    static const char * locations[] = {
        "shared/documents",
        "shared/downloads",
        "shared/music",
        "shared/Box",
        "shared/Dropbox",
        "removable/sdcard"
    };

    for(size_t i = 0; i < sizeof(locations)/sizeof(locations[0]); ++i) {
        const char * location = locations[i];
        QString path = FileUtils::joinPath("/accounts/1000", location);
        scanDirectory(QDir(path));
    }

    if(m_numImportedSongs == 0) {
        updateProgressUI(tr("No tracker songs found"), 100);
    } else {
        updateProgressUI(QString(tr("Imported %1 songs")).arg(m_numImportedSongs), 100);
    }

    Analytics::getInstance()->importedSongCount(m_numImportedSongs);

    completeProgressUI();
    return m_numImportedSongs;
}

int Importer::scanDirectory(QDir const& root)
{
#ifdef DETAILED_LOG
    qDebug() << "Filters: " << m_filters;
#endif
    QStack<QString> stack;
    stack.push(root.absolutePath());
    while(!stack.isEmpty()) {
        QString directoryPath = stack.pop();
#ifdef DETAILED_LOG
        qDebug() << "Scanning " << directoryPath;
#endif
        QString location = directoryPath;
        location.remove(0, 15);
        QString progressMessage = QString(tr("Searching for tracker songs in %1...")).arg(location);
        updateProgressUI(progressMessage, INDEFINITE);
        QDir directory(directoryPath);
#if 0
        QStringList entries = directory.entryList(m_filters,
                                                  QDir::Files | QDir::Readable | QDir::CaseSensitive,
                                                  QDir::NoSort);
#endif
        QStringList entries;
        DIR *dirp;
        if ((dirp = opendir(directoryPath.toStdString().c_str())) != NULL) {
            struct dirent64 *dp;
            do {
                if ((dp = readdir64(dirp)) != NULL) {
                    if(dp->d_name[0] == '.' && (dp->d_name[1] == '\0' || (dp->d_name[1] == '.' && dp->d_name[2] == '\0')))
                        continue;
                    for(QStringList::const_iterator i = m_filters.begin(); i != m_filters.end(); ++i) {
                        if(fnmatch(i->toStdString().c_str(), dp->d_name, 0) == 0) {
                            entries << dp->d_name;
                            break;
                        }
                    }
                }
            } while (dp != NULL);
            closedir(dirp);
        }
#ifdef DETAILED_LOG
        qDebug() << "Found songs: " << entries;
#endif
        for(int i = 0; i < entries.size(); i++) {
            QString absoluteFileName = FileUtils::joinPath(directoryPath, entries[i]);
            importFile(absoluteFileName);
        }
        QFileInfoList infoEntries = directory.entryInfoList(QStringList(),
                                                            QDir::AllDirs | QDir::NoDotAndDotDot,
                                                            QDir::NoSort);
        for (int i = 0; i < infoEntries.size(); i++) {
            stack.push(infoEntries[i].absoluteFilePath());
        }
   }
   return m_numImportedSongs;
}

bool Importer::importFile(QString const& fileName)
{
    QString fileNameOnly = FileUtils::fileNameOnly(fileName);
    QString progressMessage = QString(tr("Importing %1")).arg(fileNameOnly);
    updateProgressUI(progressMessage, INDEFINITE);

    QFile inputFile(fileName);
    if(!inputFile.exists()) {
        return false;
    }

    if(!inputFile.open(QFile::ReadOnly)) {
        qDebug() << "Failed to open input file" << fileName;
        return false;
    }

    QByteArray data = inputFile.readAll();
    if(data.size() == 0) {
        qDebug() << "Failed to read file" << fileName;
        return false;
    }

    ModPlugFile* module = ::ModPlug_Load(data.data(), data.size());
    if(module == NULL) {
        qDebug() << "Failed to load module" << fileName;
        return false;
    }

    ++m_numImportedSongs;
    SongExtendedInfo info(NULL);
    info.setId(-m_numImportedSongs);
    info.setFileName(fileName);
    info.setFileSize(data.size());
    info.setSongLength(::ModPlug_GetLength(module));
    info.setTitle(::ModPlug_GetName(module));
    info.setFormatId(SongFormat::getFormatIdByFileName(fileName));

    info.setInstruments(::ModPlug_NumInstruments(module));
    info.setChannels(::ModPlug_NumChannels(module));
    info.setSamples(::ModPlug_NumSamples(module));
    info.setPatterns(::ModPlug_NumPatterns(module));
    info.setOrders(::ModPlug_NumOrders(module));

    info.setFormat("");
    info.setTracker("");
    info.setGenre("");
    info.setArtistId(0);
    info.setArtist("");

    ::ModPlug_Unload(module);

    m_catalog->addPersonalSong(info);
    return true;
}

void Importer::destroyProgressUI() {
    if(m_progress != NULL) {
        m_progress->cancel();
        delete m_progress;
        m_progress = NULL;
    }
}

void Importer::createProgressUI() {
    destroyProgressUI();
    m_progress = new SystemProgressDialog(0);
    m_progress->setModality(SystemUiModality::Application);
    m_progress->setState(SystemUiProgressState::Active);
    m_progress->setTitle(tr("Importing Tracker Songs"));
    m_progress->confirmButton()->setEnabled(false);
    m_progress->setProgress(INDEFINITE);
    m_progress->show();
}

void Importer::completeProgressUI() {
    if(m_progress) {
        m_progress->confirmButton()->setEnabled(true);
        m_progress->exec();
    }
}

void Importer::updateProgressUI(QString const& body, int progress) {
#ifdef DETAILED_LOG
    qDebug() << body;
#endif
    if(m_progress) {
        m_progress->setBody(body);
        m_progress->setProgress(progress);
        m_progress->show();
    }
}
