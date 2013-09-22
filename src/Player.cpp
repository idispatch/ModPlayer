#include <QDeclarativeComponent>
#include <bb/multimedia/NowPlayingConnection>
#include <bb/cascades/pickers/FilePicker>
#include "FileUtils.hpp"
#include "Player.hpp"
#include "Catalog.hpp"
#include "Cache.hpp"
#include "Downloader.hpp"
#include "Unpacker.hpp"
#include "SongModule.hpp"
#include "PlaybackConfig.hpp"
#include "ModPlayback.hpp"
#include "Analytics.hpp"

using namespace bb::multimedia;

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
    "*.okt"
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
      m_nowPlaying(new NowPlayingConnection("ModPlayer", this)){
    initCache();
    initDownloader();
    initPlayback();
    initNowPlaying();
}

Player::~Player() {
    if (m_playback != NULL) {
        m_playback->stopThread();
    }
}

void Player::initCache() {
    QStringList fileNameFilters;
    for(size_t i = 0;
        i < sizeof(g_song_extensions_array)/sizeof(g_song_extensions_array[0]);
        ++i) {
        fileNameFilters << g_song_extensions_array[i];
    }
    //qDebug() << *m_cache;
    m_cache->setFileNameFilters(fileNameFilters);
    //qDebug() << *m_cache;
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
    Q_UNUSED(rc);

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
    if(QFile::remove(fileName))
    {
        //qDebug() << "Deleted" << fileName;
    }
    else
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

SongModule * Player::currentSong() const {
    return m_playback->currentSong();
}

bool Player::beginPlay(bool fromCatalog, QString const& fileName) {
    bool rv = false;
    QString fileNamePart = FileUtils::fileNameOnly(fileName);
    //qDebug() << "Player::beginPlay:" << fileName;
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

        //qDebug() << "Player::beginPlay(absoluteFileName):" << absoluteFileName;
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
        //qDebug() << "Player::play:" << value;
        SongExtendedInfo * info = songExtendedInfo(value);
        if(info != 0) {
            //qDebug() << "Player::play:" << info->fileName();
            playByModuleFileName(info->fileName());
        } else {
            qDebug() << "Player::play: Unsupported variant type:" << value;
        }
    }
}

void Player::playByModuleFileName(QString const& fileName) {
    //qDebug() << "Player::playByModuleFileName" << fileName;
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

void Player::load() {
    using namespace bb::cascades::pickers;
    bool rc;
    FilePicker* filePicker = new FilePicker();
    filePicker->setType(FileType::Other);
    filePicker->setTitle(tr("Select Song Module"));
    filePicker->setMode(FilePickerMode::Picker);

    QStringList fileNameFilters;
    for(size_t i = 0;
            i < sizeof(g_song_extensions_array)/sizeof(g_song_extensions_array[0]);
            ++i) {
            fileNameFilters << g_song_extensions_array[i];
        }
    filePicker->setFilter(fileNameFilters);

    rc = QObject::connect(filePicker, SIGNAL(fileSelected(const QStringList&)),
                          this,       SLOT(onSongLoadSelected(const QStringList&)));
    Q_ASSERT(rc);

    rc = QObject::connect(filePicker, SIGNAL(canceled()),
                          this,       SLOT(onSongLoadCanceled()));
    Q_ASSERT(rc);

    filePicker->open();
}

void Player::onSongLoadSelected(const QStringList& fileList) {
    if(fileList.size() > 0)
    {
        QString const& fileName = fileList[0];
        if(beginPlay(false, fileName))
        {
            emit localSongLoaded();
        }
    }
    sender()->deleteLater();
}

void Player::onSongLoadCanceled() {
    sender()->deleteLater();
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
