#include <QDeclarativeComponent>
#include <bb/multimedia/NowPlayingConnection>
#include "Player.hpp"
#include "Catalog.hpp"
#include "Cache.hpp"
#include "Downloader.hpp"
#include "Unpacker.hpp"
#include "SongModule.hpp"

using namespace bb::multimedia;

Player::Player(QObject * parent)
    : QObject(parent),
      m_state(Stopped),
      m_statusText("Stopped"),
      m_catalog(new Catalog(this)),
      m_cache(new Cache(this)),
      m_downloader(new Downloader(this)),
      m_unpacker(new Unpacker(this)),
      m_module(new SongModule(this)),
      m_nowPlaying(new NowPlayingConnection("ModPlayer", this)){
    initCatalog();
    initCache();
    initDownloader();
    initModule();
    initNowPlaying();
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

void Player::initModule() {
    bool rc;
    qmlRegisterUncreatableType<SongModule>("player", 1, 0, "Module", "");
    rc = connect(m_module,
                 SIGNAL(playing()),
                 this,
                 SLOT(onPlaying()));
    Q_ASSERT(rc);

    rc = connect(m_module,
                 SIGNAL(paused()),
                 this,
                 SLOT(onPaused()));
    Q_ASSERT(rc);

    rc = connect(m_module,
                 SIGNAL(stopped()),
                 this,
                 SLOT(onStopped()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
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
#if 0
        qDebug() << "There is no unpacked file returned";
#endif
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

void Player::onNowPlayingAcquired() {
    qDebug() << "Player::onNowPlayingAcquired";
    m_nowPlaying->setDuration(180000);
    m_nowPlaying->setPosition(0);
    QVariantMap metadata;

    metadata[MetaData::Title] = m_module->fileName();
    metadata[MetaData::Artist] = m_module->title();

    m_nowPlaying->setOverlayStyle(OverlayStyle::Fancy);
    m_nowPlaying->setNextEnabled(false);
    m_nowPlaying->setPreviousEnabled(false);
    m_nowPlaying->setMetaData(metadata);
    //m_nowPlaying->setIconUrl(QUrl("file://%1/app/native/assets/artwork.png").arg(QDir::currentPath()));
}

void Player::onNowPlayingRevoked() {
    qDebug() << "Player::onNowPlayingRevoked";
}

void Player::onNowPlayingPlay() {
    qDebug() << "Player::onNowPlayingPlay";
    resume();
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

SongModule * Player::currentSong() const {
    return m_module;
}

void Player::beginPlay(QString const& fileName) {
    stop();
    QString absoluteFileName = joinPath(m_cache->cachePath(), fileName);
    if(m_module->load(absoluteFileName))
    {
        if(m_module->play())
        {
            QString file = m_module->fileName();
            changeStatus(Playing, QString("Playing %1").arg(file));
            m_catalog->play(file);

            m_nowPlaying->acquire();
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
}

void Player::playByModuleFileName(QString const& fileName) {
#if 0
    qDebug() << "Player::playByModuleFileName: Playing file named" << fileName;
#endif
    if(m_cache->exists(fileName))
    {
#if 0
        qDebug() << "File" << fileName << "is already in cache";
#endif
        beginPlay(fileName);
    }
    else
    {
#if 0
        qDebug() << "File" << fileName << "is not in cache";
#endif
        QString name = fileNameOnly(fileName);
        changeStatus(Resolving, QString("Resolving %1").arg(name));

        int modId = m_catalog->resolveModuleIdByFileName(fileName);
        m_downloader->download(modId);
    }
}

void Player::playByModuleId(int modId) {
#if 0
    qDebug() << "Player::playByModuleId: Playing module id" << modId;
#endif
    QString fileName = m_catalog->resolveFileNameById(modId);
    if(m_cache->exists(fileName)) {
#if 0
        qDebug() << "File" << fileName << "(id=" << modId << ") is already in cache";
#endif
        beginPlay(fileName);
    }
    else
    {
#if 0
        qDebug() << "File" << fileName << "(id=" << modId << ") is not in cache";
#endif
        changeStatus(Downloading, "Downloading song");
        m_downloader->download(modId);
    }
}

void Player::stop() {
    m_module->stop();
}

void Player::pause() {
    m_module->pause();
}

void Player::resume() {
    m_module->resume();
}

void Player::onPaused() {
    changeStatus(Paused, "Paused");
    m_nowPlaying->setMediaState(MediaState::Paused);
}

void Player::onPlaying() {
    changeStatus(Playing, QString("Playing %1").arg(m_module->fileName()));
    m_nowPlaying->setMediaState(MediaState::Started);
}

void Player::onStopped() {
    changeStatus(Stopped, "Stopped");
    m_nowPlaying->setMediaState(MediaState::Stopped);
}
