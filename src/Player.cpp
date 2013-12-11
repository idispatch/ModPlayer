#include <QDeclarativeComponent>
#include <bb/multimedia/NowPlayingConnection>
#include <bb/cascades/pickers/FilePicker>
#include <bb/system/SystemToast>
#include "FileUtils.hpp"
#include "Player.hpp"
#include "Catalog.hpp"
#include "Cache.hpp"
#include "Downloader.hpp"
#include "Unpacker.hpp"
#include "SongModule.hpp"
#include "PlaybackConfig.hpp"
#include "ModPlayback.hpp"
#include "Playlist.hpp"
#include "SuspendPlayback.hpp"
#include "Mp3Export.hpp"
#include "Importer.hpp"
#include "Analytics.hpp"

using namespace bb::multimedia;
using namespace bb::system;

template<>
int InstanceCounter<Player>::s_count;
template<>
int InstanceCounter<Player>::s_maxCount;

static const char * g_song_extensions_array[] = {
    "*.mod",
    "*.med",
    "*.mt2",
    "*.mtm",
    "*.s3m",
    "*.it",
    "*.stm",
    "*.xm",
    "*.669",
    "*.oct",
    "*.okt",

    "*.MOD",
    "*.MED",
    "*.MT2",
    "*.MTM",
    "*.S3M",
    "*.IT",
    "*.STM",
    "*.XM",
    "*.OCT",
    "*.OKT"
};

Player::Player(QSettings &settings, QObject * parent)
    : QObject(parent),
      m_settings(settings),
      m_state(Stopped),
      m_statusText(tr("Stopped")),
      m_catalog(new Catalog(this)),
      m_cache(new Cache(m_settings, this)),
      m_downloader(new Downloader(this)),
      m_unpacker(new Unpacker(this)),
      m_playback(new ModPlayback(settings, this)),
      m_playlist(new Playlist(Playlist::PlaylistOnce, this)),
      m_nowPlaying(new NowPlayingConnection("ModPlayer", this)){
    initCache();
    initDownloader();
    initPlayback();
    initNowPlaying();
    initCatalog();
}

Player::~Player() {
    m_settings.setValue("player/mode", (int)m_playlist->mode());
    if(m_catalog != NULL) {
        m_catalog->stopThread();
    }
    if(m_playback != NULL) {
        m_playback->stopThread();
    }
}

void Player::initCatalog() {
    m_catalog->start(QThread::LowPriority);
}

void Player::initCache() {
    bool rc;
    rc = QObject::connect(m_cache, SIGNAL(currentFilesChanged()),
                          this,    SLOT(onCurrentFilesChanged()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    QStringList fileNameFilters;
    for(size_t i = 0;
        i < sizeof(g_song_extensions_array)/sizeof(g_song_extensions_array[0]);
        ++i) {
        fileNameFilters << g_song_extensions_array[i];
    }
    m_cache->setFileNameFilters(fileNameFilters);
}

void Player::initDownloader() {
    bool rc;
    rc = QObject::connect(m_downloader, SIGNAL(downloadStarted(int)),
                          this,         SLOT(onDownloadStarted(int)));
    Q_ASSERT(rc);

    rc = QObject::connect(m_downloader, SIGNAL(downloadFinished(QString)),
                          this,         SLOT(onDownloadFinished(QString)));
    Q_ASSERT(rc);

    rc = QObject::connect(m_downloader, SIGNAL(downloadFailure(int)),
                          this,         SLOT(onDownloadFailure(int)));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void Player::initPlayback() {
    bool rc;
    rc = QObject::connect(m_playback, SIGNAL(playing()),
                          this,       SLOT(onPlaying()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_playback, SIGNAL(paused()),
                          this,       SLOT(onPaused()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_playback, SIGNAL(stopped()),
                          this,       SLOT(onStopped()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_playback, SIGNAL(finished()),
                          this,       SLOT(onFinished()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    int mode = m_settings.value("player/mode", Playlist::PlaylistOnce).toInt();
    m_playlist->setMode(static_cast<Playlist::Mode>(mode));
    m_playback->start(QThread::HighPriority);
}

void Player::initNowPlaying() {
    bool rc;
    rc = QObject::connect(m_nowPlaying, SIGNAL(acquired()),
                          this,         SLOT(onNowPlayingAcquired()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_nowPlaying, SIGNAL(revoked()),
                          this,         SLOT(onNowPlayingRevoked()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_nowPlaying, SIGNAL(pause()),
                          this,         SLOT(onNowPlayingPause()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_nowPlaying, SIGNAL(play()),
                          this,         SLOT(onNowPlayingPlay()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_nowPlaying, SIGNAL(stop()),
                          this,         SLOT(onNowPlayingStop()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void Player::changeStatus(State state, QString const& statusText) {
    if(m_state != state)
    {
        m_state = state;
        if(m_nowPlaying->isAcquired())
        {
            switch(m_state) {
            case Paused:
                m_nowPlaying->setMediaState(MediaState::Paused);
                break;
            case Playing:
                m_nowPlaying->setMediaState(MediaState::Started);
                break;
            case Stopped:
                m_nowPlaying->setMediaState(MediaState::Stopped);
                break;
            case Resolving:
            case Downloading:
            case Preparing:
            default:
                m_nowPlaying->setMediaState(MediaState::Unprepared);
                break;
            }
        }
        emit stateChanged();
    }
    if(m_statusText != statusText)
    {
        m_statusText = statusText;
        emit statusTextChanged();
    }
}

void Player::onDownloadStarted(int id) {
    Q_UNUSED(id);
    changeStatus(Preparing, tr("Downloading song"));
    Analytics::getInstance()->downloadStarted();
}

void Player::onDownloadFinished(QString fileName) {
    Analytics::getInstance()->downloadFinished();

    QString relativeName = FileUtils::fileNameOnly(fileName);
    changeStatus(Preparing, QString(tr("Unpacking song %1")).arg(relativeName));

    QString unpackedFileName = m_unpacker->unpackFile(fileName);
    if(!QFile::remove(fileName))
    {
        QString message = QString("Failed to delete %1").arg(fileName);
        qDebug() << message;
        Analytics::getInstance()->logError("DeleteError", message);
    }

    if(unpackedFileName.isEmpty())
    {
        changeStatus(Stopped, QString(tr("Failed to prepare song %1")).arg(relativeName));
        return;
    }

    QString unpackedRelativeFileName = FileUtils::fileNameOnly(unpackedFileName);
    changeStatus(Preparing, QString(tr("Caching song %1")).arg(unpackedRelativeFileName));

    m_cache->cache(unpackedRelativeFileName);

    beginPlay(true, unpackedFileName);
}

void Player::onDownloadFailure(int id) {
    changeStatus(Stopped, tr("Failed to download song"));
    Analytics::getInstance()->downloadFailed(id);
    stop();
}

void Player::updateNowPlaying() {
    m_nowPlaying->setDuration(180000);
    m_nowPlaying->setPosition(0);
    QVariantMap metadata;

    metadata[MetaData::Title] = FileUtils::fileNameOnly(currentSong()->fileName());
    metadata[MetaData::Artist] = currentSong()->title();

    m_nowPlaying->setOverlayStyle(OverlayStyle::Fancy);
    m_nowPlaying->setNextEnabled(false);
    m_nowPlaying->setPreviousEnabled(false);
    m_nowPlaying->setMetaData(metadata);
    m_nowPlaying->setIconUrl(currentSong()->iconPath());
}

void Player::onNowPlayingAcquired() {
    updateNowPlaying();
}

void Player::onNowPlayingRevoked() {
}

void Player::onNowPlayingPlay() {
    if(state() == Paused)
    {
        resume();
    }
    else if(state() == Stopped)
    {
        m_playback->play();
    }
}

void Player::onNowPlayingStop() {
    stop();
}

void Player::onNowPlayingPause() {
    pause();
}

Player::State Player::state() const {
    return m_state;
}

QString Player::statusText() const {
    return m_statusText;
}

QString Player::userDirectory() const {
    return m_userDirectory;
}

void Player::setUserDirectory(QString const& value) {
    if(m_userDirectory != value) {
        m_userDirectory = value;
        emit userDirectoryChanged();
    }
}

Catalog * Player::catalog() const {
    return m_catalog;
}

Cache * Player::cache() const {
    return m_cache;
}

Downloader * Player::downloader() const {
    return m_downloader;
}

ModPlayback * Player::playback() const {
    return m_playback;
}

Playlist * Player::playlist() const {
    return m_playlist;
}

SongModule * Player::currentSong() const {
    return m_playback->currentSong();
}

bool Player::beginPlay(bool fromCatalog, QString const& fileName) {
    bool rv = false;
    QString fileNamePart = FileUtils::fileNameOnly(fileName);
    SongExtendedInfo * info = NULL;

    if(fromCatalog) {
        info = m_catalog->resolveModuleByFileName(fileNamePart, QVariant());
    } else {
        info = new SongExtendedInfo(NULL);
        info->setFileName(fileName);
    }

    if(info != NULL)
    {
        QString absoluteFileName;
        if(fromCatalog) {
            absoluteFileName = FileUtils::joinPath(m_cache->cachePath(), fileName);
        } else {
            absoluteFileName = fileName;
        }

        if(m_playback->load(*info, absoluteFileName))
        {
            if(m_playback->play())
            {
                if(fromCatalog) {
                    m_catalog->play(QVariant::fromValue(static_cast<QObject*>(currentSong())));
                }

                QString relativeFileName = FileUtils::fileNameOnly(currentSong()->fileName());
                changeStatus(Playing, QString(tr("Playing %1")).arg(relativeFileName));

                if(!m_nowPlaying->isAcquired())
                {
                    m_nowPlaying->acquire();
                }
                else
                {
                    updateNowPlaying();
                }

                rv = true;
            }
            else
            {
                QString message = QString("Failed to play %1").arg(absoluteFileName);
                qDebug() << message;
                Analytics::getInstance()->logError("PlayError", message);
            }
        }
        else
        {
            QString message = QString("Failed to load %1").arg(absoluteFileName);
            qDebug() << message;
            Analytics::getInstance()->logError("LoadError", message);
        }

        delete info;
    }
    else
    {
        QString message = QString("Failed to resolve %1").arg(fileNamePart);
        qDebug() << message;
        Analytics::getInstance()->logError("LoadError", message);
    }

    return rv;
}

void Player::play(QVariant value) {
    if(value.type() == QVariant::Int)
    {
        playByModuleId(value.toInt());
    }
    else if(value.type() == QVariant::String)
    {
        QString valueString = value.toString();
        bool parseableInt = false;
        int id = valueString.toInt(&parseableInt, 10);
        if(parseableInt)
        {
            playByModuleId(id);
        }
        else
        {
            playByModuleFileName(valueString);
        }
    }
    else {
        SongExtendedInfo * info = songExtendedInfo(value);
        if(info != 0) {
            playByModuleFileName(info->fileName());
        } else {
            qDebug() << "Player::play: Unsupported variant type:" << value;
        }
    }
}

void Player::playByModuleFileName(QString const& fileName) {
    // relative path or within cache directory - play from cache
    if(fileName.startsWith(m_cache->cachePath()) || !fileName.startsWith("/")) {
        if(m_cache->exists(fileName)) {
            beginPlay(true, fileName);
        }
        else
        {
            QString name = FileUtils::fileNameOnly(fileName);
            changeStatus(Resolving, QString(tr("Resolving %1")).arg(name));

            int id = m_catalog->resolveModuleIdByFileName(fileName);
            m_downloader->download(id);
        }
    } else {
        // otherwise play from the absolute path
        beginPlay(false, fileName);
    }
}

void Player::playByModuleId(int id) {
    QString fileName = m_catalog->resolveFileNameById(id);
    if(m_cache->exists(fileName)) {
        beginPlay(true, fileName);
    }
    else
    {
        changeStatus(Downloading, tr("Downloading song"));
        m_downloader->download(id);
    }
}

void Player::stop() {
    m_playback->stop();
}

void Player::pause() {
    m_playback->pause();
}

void Player::resume() {
    m_playback->resume();
}

void Player::browseForLocalSong() {
    using namespace bb::cascades::pickers;
    bool rc;
    FilePicker* filePicker = new FilePicker();
    filePicker->setType(FileType::Other);
    filePicker->setTitle(tr("Select Song Module"));
    filePicker->setMode(FilePickerMode::Picker);
    if(m_userDirectory.length() > 0) {
        QStringList directories;
        directories << m_userDirectory;
        filePicker->setDirectories(directories);
    }

    QStringList fileNameFilters;
    for(size_t i = 0;
            i < sizeof(g_song_extensions_array)/sizeof(g_song_extensions_array[0]);
            ++i) {
        fileNameFilters << g_song_extensions_array[i];
    }
    filePicker->setFilter(fileNameFilters);

    rc = QObject::connect(filePicker, SIGNAL(fileSelected(const QStringList&)),
                          this,       SLOT(onLocalSongSelected(const QStringList&)));
    Q_ASSERT(rc);

    rc = QObject::connect(filePicker, SIGNAL(canceled()),
                          this,       SLOT(onLocalSongBrowseCanceled()));
    Q_ASSERT(rc);

    filePicker->open();
}

void Player::onLocalSongSelected(const QStringList& fileList) {
    if(fileList.size() == 1)
    {
        QString songFilePath(fileList[0]);
        QString directory = FileUtils::directoryOnly(songFilePath);
        setUserDirectory(directory);
        playLocalSong(songFilePath);
    }
    else if(fileList.size() > 1)
    {
        qDebug() << "Selected multiple songs:" << fileList.size();
    }
    sender()->deleteLater();
}

void Player::onLocalSongBrowseCanceled() {
    sender()->deleteLater();
}

void Player::playLocalSong(QString const& fileName) {
    beginPlay(false, fileName);
    emit requestPlayerView();
}

void Player::onPaused() {
    changeStatus(Paused, tr("Paused"));
}

void Player::onPlaying() {
    QString songFileName = currentSong()->fileName();
    QString songRelativeFileName = FileUtils::fileNameOnly(songFileName);
    changeStatus(Playing, QString(tr("Playing %1")).arg(songRelativeFileName));
}

void Player::onStopped() {
    changeStatus(Stopped, tr("Stopped"));
}

void Player::onFinished() {
    switch(m_playlist->mode()){
    case Playlist::SongOnce:
        m_playlist->reset();
        break;
    case Playlist::SongCycle:
        m_playback->play();
        break;
    case Playlist::PlaylistOnce:
    case Playlist::PlaylistRandomOnce:
    case Playlist::PlaylistCycle:
    case Playlist::PlaylistRandomCycle:
        if(m_playlist->atEnd()) {
            m_playlist->reset();
        } else {
            playByModuleId(m_playlist->next());
	}
        break;
    default:
        break;
    }
}

void Player::timerEvent(QTimerEvent *event) {
    killTimer(event->timerId());
    SystemToast toast;
    toast.setBody(tr("Please support ModPlayer - write a review in BlackBerry World!"));
    toast.setModality(SystemUiModality::Application);
    toast.setPosition(SystemUiPosition::MiddleCenter);
    toast.button()->setLabel("Ok");
    toast.exec();
}

void Player::onCurrentFilesChanged() {
    if(m_cache->currentFiles() == 5) {
        startTimer(3000);
    }
}

void Player::exportMp3(QString const& inputFileName,
                       QString const& outputFileName) {
    Analytics::getInstance()->exportMp3(inputFileName, outputFileName);
    SuspendPlayback suspender(playback());
    Mp3Export exporter;
    exporter.convert(*m_playback->configuration(),
                     inputFileName,
                     outputFileName);
}

void Player::importSongs() {
    Analytics::getInstance()->importSongs(true);
    SuspendPlayback suspender(playback());
    try {
        QStringList fileNameFilters;
        for(size_t i = 0;
                i < sizeof(g_song_extensions_array)/sizeof(g_song_extensions_array[0]);
                ++i) {
            fileNameFilters << g_song_extensions_array[i];
        }
        Importer importer(fileNameFilters, catalog());
        importer.import();
    } catch(...) {

    }
    Analytics::getInstance()->importSongs(false);
}

