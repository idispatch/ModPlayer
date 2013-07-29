#include <QDeclarativeComponent>
#include "Player.hpp"
#include "Catalog.hpp"
#include "Cache.hpp"
#include "Downloader.hpp"
#include "Unpacker.hpp"
#include "SongModule.hpp"

Player::Player(QObject * parent)
    : QObject(parent),
      m_state(Stopped),
      m_statusText("Stopped"),
      m_catalog(new Catalog(this)),
      m_cache(new Cache(this)),
      m_downloader(new Downloader(this)),
      m_unpacker(new Unpacker(this)),
      m_module(new SongModule(this)) {
    initCatalog();
    initCache();
    initDownloader();
    initModule();
}

QString Player::fileNameOnly(QString const& fileName) {
    QFile file(fileName);
    QFileInfo fileInfo(file.fileName());
    return fileInfo.fileName();
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
    qmlRegisterUncreatableType<Downloader>("player", 1, 0, "Downloader", "");
    connect(m_downloader,
            SIGNAL(downloadStarted(int)),
            this,
            SLOT(downloadStarted(int)));
    connect(m_downloader,
            SIGNAL(downloadFinished(QString)),
            this,
            SLOT(downloadFinished(QString)));
    connect(m_downloader,
            SIGNAL(downloadFailure(int)),
            this,
            SLOT(downloadFailure(int)));
}

void Player::initModule() {
    qmlRegisterUncreatableType<SongModule>("player", 1, 0, "Module", "");
    connect(m_module,
            SIGNAL(playing()),
            this,
            SLOT(playing()));
    connect(m_module,
            SIGNAL(paused()),
            this,
            SLOT(paused()));
    connect(m_module,
            SIGNAL(stopped()),
            this,
            SLOT(stopped()));
}

void Player::changeStatus(State state, QString const& statusText) {
    if(m_state != state)
    {
        m_state = state;
        qDebug() << "State changed, new state" << m_state;
        emit stateChanged();
    }
    if(m_statusText != statusText)
    {
        m_statusText = statusText;
        qDebug() << m_statusText;
        emit statusTextChanged();
    }
}

void Player::downloadStarted(int modId) {
    Q_UNUSED(modId);
    changeStatus(Preparing, "Downloading song");
}

void Player::downloadFinished(QString fileName) {
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
        qDebug() << "There is no unpacked file returned";

        changeStatus(Stopped, QString("Failed to prepare song %1").arg(name));
        return;
    }

    changeStatus(Preparing, QString("Caching song %1").arg(name));

    m_cache->cache(newFile);

    beginPlay(newFile);
}

void Player::downloadFailure(int modId) {
    Q_UNUSED(modId);
    stop();
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
    qDebug() << "Player::beginPlay";
    stop();
    qDebug() << "Module loading...";
    if(m_module->load(fileName))
    {
        if(m_module->play())
        {
            changeStatus(Playing, QString("Playing %1").arg(m_module->fileName()));
        }
    }
}

void Player::play(QVariant value) {
    qDebug() << "Playing" << value;

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
    qDebug() << "Playing module file name=" << fileName;

    if(m_cache->exists(fileName))
    {
        beginPlay(fileName);
    }
    else
    {
        QString name = fileNameOnly(fileName);
        changeStatus(Resolving, QString("Resolving %1").arg(name));
    }
}

void Player::playByModuleId(int modId) {
    qDebug() << "Playing module id=" << modId;

    changeStatus(Downloading, "Downloading song");
    m_downloader->download(modId);
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

void Player::paused() {
    changeStatus(Paused, QString("Paused %1").arg(m_module->fileName()));
}

void Player::playing() {
    changeStatus(Playing, QString("Playing %1").arg(m_module->fileName()));
}

void Player::stopped() {
    changeStatus(Stopped, "Stopped");
}
