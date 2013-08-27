#include <QDeclarativeComponent>
#include <bb/multimedia/NowPlayingConnection>
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

QString Player::joinPath(QString const& directory, QString const& fileName) {
    if(fileName.startsWith('/')) {
        return fileName; // already absolute
    }
    QString result = directory;
    if(directory.endsWith('/')) {
        result += fileName;
    } else {
        result = result + "/" + fileName;
    }
    return result;
}

QString Player::fileNameOnly(QString const& fileName) {
    int pos = fileName.lastIndexOf('/');
    if(pos == -1) {
        return fileName;
    } else {
        return fileName.mid(pos + 1);
    }
}

void Player::initCache() {
    QStringList fileNameFilters;
    fileNameFilters << "*.mod"
                    << "*.med"
                    << "*.mt2"
                    << "*.mtm"
                    << "*.s3m"
                    << "*.it"
                    << "*.stm"
                    << "*.xm"
                    << "*.669"
                    << "*.oct"
                    << "*.okt";
    qDebug() << *m_cache;
    m_cache->setFileNameFilters(fileNameFilters);
    qDebug() << *m_cache;
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
}

void Player::onDownloadFinished(QString fileName) {
    QString name = fileNameOnly(fileName);
    changeStatus(Preparing, QString(tr("Unpacking song %1")).arg(name));

    QString newFile = m_unpacker->unpackFile(fileName);
    if(QFile::remove(fileName))
    {
        qDebug() << "Deleted" << fileName;
    }
    else
    {
        qDebug() << "Failed to delete" << fileName;
    }

    if(newFile.isEmpty())
    {
        changeStatus(Stopped, QString(tr("Failed to prepare song %1")).arg(name));
        return;
    }

    QString file = fileNameOnly(newFile);
    changeStatus(Preparing, QString(tr("Caching song %1")).arg(file));

    m_cache->cache(file);

    beginPlay(newFile);
}

void Player::onDownloadFailure(int id) {
    changeStatus(Stopped, tr("Failed to download song"));
    Analytics::getInstance()->failedDownload(id);
    stop();
}

void Player::updateNowPlaying() {
    m_nowPlaying->setDuration(180000);
    m_nowPlaying->setPosition(0);
    QVariantMap metadata;

    metadata[MetaData::Title] = currentSong()->fileName();
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

void Player::beginPlay(QString const& fileName) {
    QString fileNamePart = fileNameOnly(fileName);
    SongExtendedInfo * info = m_catalog->resolveModuleByFileName(fileNamePart, QVariant());
    if(info != NULL)
    {
        QString absoluteFileName = joinPath(m_cache->cachePath(), fileName);
        if(m_playback->load(*info, absoluteFileName))
        {
            if(m_playback->play())
            {
                QString file = currentSong()->fileName();
                changeStatus(Playing, QString(tr("Playing %1")).arg(file));

                m_catalog->play(QVariant::fromValue(static_cast<QObject*>(currentSong())));

                if(!m_nowPlaying->isAcquired())
                {
                    m_nowPlaying->acquire();
                }
                else
                {
                    updateNowPlaying();
                }
            }
            else
            {
                qDebug() << "Failed to play:" << absoluteFileName;
            }
        }
        else
        {
            qDebug() << "Failed to load:" << absoluteFileName;
        }

        delete info;
    }
    else
    {
        qDebug() << "Failed to resolve:" << fileNamePart;
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
        }
    }
}

void Player::playByModuleFileName(QString const& fileName) {
    if(m_cache->exists(fileName))
    {
        beginPlay(fileName);
    }
    else
    {
        QString name = fileNameOnly(fileName);
        changeStatus(Resolving, QString(tr("Resolving %1")).arg(name));

        int id = m_catalog->resolveModuleIdByFileName(fileName);
        m_downloader->download(id);
    }
}

void Player::playByModuleId(int id) {
    QString fileName = m_catalog->resolveFileNameById(id);
    if(m_cache->exists(fileName)) {
        beginPlay(fileName);
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

void Player::onPaused() {
    changeStatus(Paused, tr("Paused"));
}

void Player::onPlaying() {
    changeStatus(Playing, QString(tr("Playing %1")).arg(currentSong()->fileName()));
}

void Player::onStopped() {
    changeStatus(Stopped, tr("Stopped"));
}
