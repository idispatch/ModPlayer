#include <QDeclarativeComponent>
#include <bb/multimedia/NowPlayingConnection>
#include <bb/cascades/pickers/FilePicker>
#include <bb/system/SystemToast>
#include <bb/system/SystemDialog>
#include "FileUtils.hpp"
#include "Player.hpp"
#include "Catalog.hpp"
#include "Cache.hpp"
#include "Downloader.hpp"
#include "Unpacker.hpp"
#include "SongModule.hpp"
#include "PlaybackConfig.hpp"
#include "Playback.hpp"
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

Player::Player(QSettings &settings, QObject * parent)
    : QObject(parent),
      m_feedbackTimerId(-1),
      m_importTimerId(-1),
      m_settings(settings),
      m_state(Stopped),
      m_statusText(tr("Stopped")),
      m_catalog(new Catalog(this)),
      m_cache(new Cache(m_settings, this)),
      m_downloader(new Downloader(this)),
      m_unpacker(new Unpacker(this)),
      m_playback(new Playback(settings, this)),
      m_playlist(new Playlist(Playlist::PlaylistOnce, this)),
      m_importer(NULL),
      m_nowPlaying(new NowPlayingConnection("ModPlayer", this)){
    m_fileNameFilters << "*.mod" << "*.med" << "*.mt2" << "*.mtm" << "*.mp3" << "*.s3m" << "*.it" << "*.stm" << "*.xm" << "*.669" << "*.oct" << "*.okt"
                      << "*.MOD" << "*.MED" << "*.MT2" << "*.MTM" << "*.MP3" << "*.S3M" << "*.IT" << "*.STM" << "*.XM" << "*.OCT" << "*.OKT";
    initCache();
    initDownloader();
    initPlayback();
    initNowPlaying();
    initPlaylist();
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
#ifdef VERBOSE_LOGGING
    qDebug() << "Player::~Player()";
#endif
}

void Player::initCatalog() {
    m_catalog->start(QThread::LowPriority);
    m_importTimerId = startTimer(8000);
}

void Player::initCache() {
    bool rc;
    rc = QObject::connect(m_cache, SIGNAL(currentFilesChanged()),
                          this,    SLOT(onCurrentFilesChanged()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    m_cache->setFileNameFilters(m_fileNameFilters);
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

    rc = QObject::connect(m_nowPlaying, SIGNAL(next()),
                          this,         SLOT(onNowPlayingNext()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_nowPlaying, SIGNAL(previous()),
                          this,         SLOT(onNowPlayingPrevious()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void Player::initPlaylist() {
    bool rc;
    rc = QObject::connect(m_playlist, SIGNAL(nextAvailableChanged()),
                          this,       SLOT(onPlaylistNextAvailableChanged()));
    Q_ASSERT(rc);

    rc = QObject::connect(m_playlist, SIGNAL(previousAvailableChanged()),
                          this,       SLOT(onPlaylistPreviousAvailableChanged()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void Player::onPlaylistNextAvailableChanged() {
    if(m_nowPlaying!=NULL && m_playlist!=NULL) {
        m_nowPlaying->setNextEnabled(m_playlist->nextAvailable());
    }
}

void Player::onPlaylistPreviousAvailableChanged() {
    if(m_nowPlaying!=NULL && m_playlist!=NULL) {
        m_nowPlaying->setPreviousEnabled(m_playlist->previousAvailable());
    }
}

void Player::onNowPlayingNext() {
    playNext();
}

void Player::onNowPlayingPrevious() {
    playPrevious();
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
    changeStatus(Preparing, tr("Unpacking song %1").arg(relativeName));

    QString unpackedFileName = m_unpacker->unpackFile(fileName);
    if(!QFile::remove(fileName))
    {
        QString message = QString("Failed to delete %1").arg(fileName);
        qDebug() << message;
        Analytics::getInstance()->logError("DeleteError", message);
    }

    if(unpackedFileName.isEmpty())
    {
        changeStatus(Stopped, tr("Failed to prepare song %1").arg(relativeName));
        return;
    }

    QString unpackedRelativeFileName = FileUtils::fileNameOnly(unpackedFileName);
    changeStatus(Preparing, tr("Caching song %1").arg(unpackedRelativeFileName));

    m_cache->cache(unpackedRelativeFileName);

    // Start playing from Catalog
    beginPlay(true, unpackedRelativeFileName);
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
    m_nowPlaying->setNextEnabled(m_playlist->nextAvailable());
    m_nowPlaying->setPreviousEnabled(m_playlist->previousAvailable());
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

Playback * Player::playback() const {
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

    std::auto_ptr<SongExtendedInfo> info;

    if(fromCatalog) {
        info.reset(m_catalog->resolveModuleByFileName(fileName, QVariant()));
    } else {
        info.reset(new SongExtendedInfo(NULL));
        info->setFileName(fileName);
    }

    if(info.get()) {
        QString absoluteFileName;
        if(fromCatalog) {
            if(FileUtils::isRelative(fileName)) {
                absoluteFileName = FileUtils::joinPath(m_cache->cachePath(), fileName);
            } else {
                absoluteFileName = fileName;
            }
        } else {
            absoluteFileName = fileName;
            info->setFileSize(QFile(absoluteFileName).size());
        }

        if(m_playback->load(*info, absoluteFileName)) {
            if(m_playback->play()) {
                if(fromCatalog) {
                    m_catalog->play(QVariant::fromValue(static_cast<QObject*>(currentSong())));
                }

                QString relativeFileName = FileUtils::fileNameOnly(currentSong()->fileName());
                changeStatus(Playing, tr("Playing %1").arg(relativeFileName));

                if(!m_nowPlaying->isAcquired()) {
                    m_nowPlaying->acquire();
                } else {
                    updateNowPlaying();
                }

                rv = true;
            } else {
                QString message = QString("Failed to play %1").arg(absoluteFileName);
                qDebug() << message;
                Analytics::getInstance()->logError("PlayError", message);
            }
        } else {
            QString message = QString("Failed to load %1").arg(absoluteFileName);
            qDebug() << message;
            Analytics::getInstance()->logError("LoadError", message);
        }
    } else {
        QString message = QString("Failed to resolve %1").arg(fileName);
        qDebug() << message;
        Analytics::getInstance()->logError("LoadError", message);
    }

    return rv;
}

void Player::playPlaylist() {
    if(m_playlist->current() != 0) {
        int songId = m_playlist->current();
        playByModuleId(songId);
    }
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
    if(fileName.startsWith(m_cache->cachePath()) || FileUtils::isRelative(fileName)) {
        if(m_cache->exists(fileName)) {
            QString name = FileUtils::fileNameOnly(fileName);
            beginPlay(true, name);
        } else {
            QString name = FileUtils::fileNameOnly(fileName);
            changeStatus(Resolving, tr("Resolving %1").arg(name));

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
    if(id > 0) {
        if(m_cache->exists(fileName)) {
            // Play cached file from Catalog
            beginPlay(true, fileName);
        } else {
            changeStatus(Downloading, tr("Downloading song"));
            m_downloader->download(id);
        }
    } else {
        // Play local file from Catalog
        beginPlay(true, fileName);
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

void Player::seek(int position) {
    m_playback->seek(position);
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

    filePicker->setFilter(m_fileNameFilters);

    rc = QObject::connect(filePicker, SIGNAL(fileSelected(const QStringList&)),
                          this,       SLOT(onLocalSongSelected(const QStringList&)));
    Q_ASSERT(rc);

    rc = QObject::connect(filePicker, SIGNAL(canceled()),
                          this,       SLOT(onLocalSongBrowseCanceled()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

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
    QString songName = FileUtils::fileNameOnly(currentSong()->fileName());
    changeStatus(Playing, tr("Playing %1").arg(songName));
}

void Player::onStopped() {
    changeStatus(Stopped, tr("Stopped"));
}

void Player::onFinished() {
    switch(m_playlist->mode()){
    case Playlist::SongCycle:
        if(m_playlist->current() != 0) {
            m_playback->play();
        }
        break;
    case Playlist::PlaylistOnce:
    case Playlist::PlaylistRandomOnce:
        if(m_playlist->nextAvailable()) {
            playByModuleId(m_playlist->next());
        } else {
            m_playlist->reset();
        }
        break;
    case Playlist::PlaylistCycle:
    case Playlist::PlaylistRandomCycle:
        if(m_playlist->nextAvailable()) {
            playByModuleId(m_playlist->next());
        }
        break;
    default:
        break;
    }
}

void Player::timerEvent(QTimerEvent *event) {
    if(event->timerId() == m_feedbackTimerId)
    {
        SystemToast toast;
        toast.setBody(tr("Please support ModPlayer - write a review in BlackBerry World!"));
        toast.setModality(SystemUiModality::Application);
        toast.setPosition(SystemUiPosition::MiddleCenter);
        toast.button()->setLabel("Ok");
        toast.exec();
        m_feedbackTimerId = -1;
    }
    else if(event->timerId() == m_importTimerId)
    {
        QDateTime date;
        if(!Importer::lastImportPerformed(date)) {
            SystemDialog dlg;
            dlg.setTitle(tr("Confirm"));
            dlg.setBody(tr("Would you like to import local songs?"));
            if(dlg.exec() == SystemUiResult::ConfirmButtonSelection) {
                importSongs();
            }
        }
        m_importTimerId = -1;
    }
    killTimer(event->timerId());
}

void Player::onCurrentFilesChanged() {
    // When use has downloaded and played 5 files ask them to support ModPlayer
    if(m_cache->currentFiles() == 5) {
        m_feedbackTimerId = startTimer(3000);
    }
}

void Player::playNext() {
    switch(m_playlist->mode()){
    case Playlist::PlaylistOnce:
    case Playlist::PlaylistRandomOnce:
        if(m_playlist->nextAvailable()) {
            playByModuleId(m_playlist->next());
        } else {
            m_playlist->reset();
        }
        break;
    case Playlist::SongCycle:
    case Playlist::PlaylistCycle:
    case Playlist::PlaylistRandomCycle:
        if(m_playlist->nextAvailable()) {
            playByModuleId(m_playlist->next());
        }
        break;
    default:
        break;
    }
}

void Player::playPrevious() {
    if(m_playlist->previousAvailable()) {
        int songId = m_playlist->previous();
        playByModuleId(songId);
    }
}

void Player::exportMp3(QString const& inputFileName,
                       QString const& outputFileName) {
    Analytics::getInstance()->exportMp3(inputFileName, outputFileName);
    m_playBackSuspend.reset(new SuspendPlayback(playback()));
    Mp3Export exporter;
    exporter.convert(*m_playback->configuration(),
                     inputFileName,
                     outputFileName);
}

void Player::importSongs() {
    Analytics::getInstance()->importSongs(true);
    m_playBackSuspend.reset(new SuspendPlayback(playback()));
    if(m_importer != NULL) {
        delete m_importer;
        m_importer = NULL;
    }
    m_importer = new Importer(m_fileNameFilters, catalog(), NULL);
    bool rc;
    Q_UNUSED(rc);
    rc = QObject::connect(m_importer, SIGNAL(searchCompleted()),
                          this,       SLOT(onImportSongsCompleted()));
    Q_ASSERT(rc);
    m_importer->start();
}

void Player::onImportSongsCompleted() {
    m_importer->deleteLater();
    m_importer = NULL;
    Analytics::getInstance()->importSongs(false);
    m_playBackSuspend.reset();
}

