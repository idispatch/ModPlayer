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

using namespace bb::multimedia;

Player::Player(QObject * parent)
    : QObject(parent),
      m_state(Stopped),
      m_statusText("Stopped"),
      m_catalog(new Catalog(this)),
      m_cache(new Cache(this)),
      m_downloader(new Downloader(this)),
      m_unpacker(new Unpacker(this)),
      m_playback(new ModPlayback(this)),
      m_nowPlaying(new NowPlayingConnection("ModPlayer", this)){
    initCatalog();
    initCache();
    initDownloader();
    initPlayback();
    initNowPlaying();
    initData();
}

Player::~Player() {
    if (m_playback != NULL) {
        m_playback->stopThread();
    }
}

void Player::initData() {
    QString appFolder(QDir::homePath());
    appFolder.chop(4); // remove data directory from end
    QString imagesDir = appFolder + "app/native/assets/images";
    m_formatIdToIconUrlMap[1] = QUrl(QString("file://") + joinPath(imagesDir, "icon_mod.png"));
    m_formatIdToIconUrlMap[2] = QUrl(QString("file://") + joinPath(imagesDir, "icon_669.png"));
    m_formatIdToIconUrlMap[3] = QUrl(QString("file://") + joinPath(imagesDir, "icon_it.png"));
    m_formatIdToIconUrlMap[4] = QUrl(QString("file://") + joinPath(imagesDir, "icon_med.png"));
    m_formatIdToIconUrlMap[5] = QUrl(QString("file://") + joinPath(imagesDir, "icon_mtm.png"));
    m_formatIdToIconUrlMap[8] = QUrl(QString("file://") + joinPath(imagesDir, "icon_s3m.png"));
    m_formatIdToIconUrlMap[9] = QUrl(QString("file://") + joinPath(imagesDir, "icon_stm.png"));
    m_formatIdToIconUrlMap[10] = QUrl(QString("file://") + joinPath(imagesDir, "icon_xm.png"));
}

QUrl Player::getIconPathByFormatId(int formatId) const {
    return m_formatIdToIconUrlMap[formatId];
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

void Player::initCatalog() {
    qmlRegisterUncreatableType<Catalog>("player", 1, 0, "Catalog", "");
}

void Player::initCache() {
    qmlRegisterUncreatableType<Cache>("player", 1, 0, "Cache", "");
    QStringList fileNameFilters;
    fileNameFilters << "*.mod"
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
    qmlRegisterUncreatableType<Downloader>("player", 1, 0, "Downloader", "");
    rc = connect(m_downloader,
                 SIGNAL(downloadStarted(int)),
                 this,
                 SLOT(onDownloadStarted(int)));
    Q_ASSERT(rc);

    rc = connect(m_downloader,
                 SIGNAL(downloadFinished(QString)),
                 this,
                 SLOT(onDownloadFinished(QString)));
    Q_ASSERT(rc);

    rc = connect(m_downloader,
                 SIGNAL(downloadFailure(int)),
                 this,
                 SLOT(onDownloadFailure(int)));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void Player::initPlayback() {
    qmlRegisterUncreatableType<SongModule>("player", 1, 0, "Module", "");
    qmlRegisterUncreatableType<ModPlayback>("player", 1, 0, "Playback", "");
    qmlRegisterUncreatableType<PlaybackConfig>("player", 1, 0, "PlaybackConfig", "");
    bool rc;
    rc = connect(m_playback,
                 SIGNAL(playing()),
                 this,
                 SLOT(onPlaying()));
    Q_ASSERT(rc);

    rc = connect(m_playback,
                 SIGNAL(paused()),
                 this,
                 SLOT(onPaused()));
    Q_ASSERT(rc);

    rc = connect(m_playback,
                 SIGNAL(stopped()),
                 this,
                 SLOT(onStopped()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    m_playback->start(QThread::HighPriority);
}

void Player::initNowPlaying() {
    bool rc;
    rc = connect(m_nowPlaying,
                 SIGNAL(acquired()),
                 this,
                 SLOT(onNowPlayingAcquired()));
    Q_ASSERT(rc);

    rc = connect(m_nowPlaying,
                 SIGNAL(revoked()),
                 this,
                 SLOT(onNowPlayingRevoked()));
    Q_ASSERT(rc);

    rc = connect(m_nowPlaying,
                 SIGNAL(pause()),
                 this,
                 SLOT(onNowPlayingPause()));
    Q_ASSERT(rc);

    rc = connect(m_nowPlaying,
                 SIGNAL(play()),
                 this,
                 SLOT(onNowPlayingPlay()));
    Q_ASSERT(rc);

    rc = connect(m_nowPlaying,
                 SIGNAL(stop()),
                 this,
                 SLOT(onNowPlayingStop()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void Player::changeStatus(State state, QString const& statusText) {
    if(m_state != state)
    {
        m_state = state;
        emit stateChanged();
    }
    if(m_statusText != statusText)
    {
        m_statusText = statusText;
        emit statusTextChanged();
    }
}

void Player::onDownloadStarted(int modId) {
    Q_UNUSED(modId);
    changeStatus(Preparing, "Downloading song");
}

void Player::onDownloadFinished(QString fileName) {
    QString name = fileNameOnly(fileName);
    changeStatus(Preparing, QString("Unpacking song %1").arg(name));

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
        changeStatus(Stopped, QString("Failed to prepare song %1").arg(name));
        return;
    }

    QString file = fileNameOnly(newFile);
    changeStatus(Preparing, QString("Caching song %1").arg(file));

    m_cache->cache(file);

    beginPlay(newFile);
}

void Player::onDownloadFailure(int modId) {
    Q_UNUSED(modId);
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
    m_nowPlaying->setIconUrl(getIconPathByFormatId(currentSong()->formatId()));
}

void Player::onNowPlayingAcquired() {
    qDebug() << "Player::onNowPlayingAcquired";
    updateNowPlaying();
}

void Player::onNowPlayingRevoked() {
    qDebug() << "Player::onNowPlayingRevoked";
}

void Player::onNowPlayingPlay() {
    qDebug() << "Player::onNowPlayingPlay";
    if(state() == Paused) {
        resume();
    } else if(state() == Stopped) {
        m_playback->play();
    }
}

void Player::onNowPlayingStop() {
    qDebug() << "Player::onNowPlayingStop";
    stop();
}

void Player::onNowPlayingPause() {
    qDebug() << "Player::onNowPlayingPause";
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
    qDebug() << "beginPlay:" << fileName;
    QString fileNamePart = fileNameOnly(fileName);
    qDebug() << "beginPlay:" << fileNamePart;
    SongInfo * info = m_catalog->resolveModuleByFileName(fileNamePart);
    if(info != NULL)
    {
        qDebug() << "beginPlay:" << info;
        QString absoluteFileName = joinPath(m_cache->cachePath(), fileName);
        if(m_playback->load(*info, absoluteFileName))
        {
            if(m_playback->play())
            {
                QString file = currentSong()->fileName();
                changeStatus(Playing, QString("Playing %1").arg(file));
                m_catalog->play(file);

                if(!m_nowPlaying->isAcquired()) {
                    m_nowPlaying->acquire();
                } else {
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
        int modId = valueString.toInt(&parseableInt, 10);
        if(parseableInt)
        {
            playByModuleId(modId);
        }
        else
        {
            playByModuleFileName(valueString);
        }
    }
    else {
        qDebug() << "play:" << value;
        SongInfo * info = qobject_cast<SongInfo*>(value.value<QObject*>());
        qDebug() << "play:" << info;
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
        changeStatus(Resolving, QString("Resolving %1").arg(name));

        int modId = m_catalog->resolveModuleIdByFileName(fileName);
        m_downloader->download(modId);
    }
}

void Player::playByModuleId(int modId) {
    QString fileName = m_catalog->resolveFileNameById(modId);
    if(m_cache->exists(fileName)) {
        beginPlay(fileName);
    }
    else
    {
        changeStatus(Downloading, "Downloading song");
        m_downloader->download(modId);
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
    changeStatus(Paused, "Paused");
    m_nowPlaying->setMediaState(MediaState::Paused);
}

void Player::onPlaying() {
    changeStatus(Playing, QString("Playing %1").arg(currentSong()->fileName()));
    m_nowPlaying->setMediaState(MediaState::Started);
}

void Player::onStopped() {
    changeStatus(Stopped, "Stopped");
    m_nowPlaying->setMediaState(MediaState::Stopped);
}
