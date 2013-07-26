#include <QDeclarativeComponent>
#include "Player.hpp"
#include "Cache.hpp"
#include "Downloader.hpp"
#include "Unpacker.hpp"
#include "SongModule.hpp"

Player::Player(QObject * parent)
    : QObject(parent),
      m_state(Stopped),
      m_statusText("Stopped"),
      m_cache(new Cache(this)),
      m_downloader(new Downloader(this)),
      m_unpacker(new Unpacker(this)),
      m_module(new SongModule(this)) {
    initCache();
    initDownloader();
    initModule();
}

QString Player::fileNameOnly(QString const& fileName) {
    QFile file(fileName);
    QFileInfo fileInfo(file.fileName());
    return fileInfo.fileName();
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
}

void Player::changeStatus(QString const& text) {
    if(text != m_statusText) {
        m_statusText = text;
        emit statusTextChanged();
    }
}

void Player::downloadStarted(int modId) {
    Q_UNUSED(modId);
    changeStatus(QString("Downloading song %1").arg(modId));
}

void Player::downloadFinished(QString fileName) {
    m_state = Preparing;
    emit stateChanged();

    QString name = fileNameOnly(fileName);
    changeStatus(QString("Unpacking song %1").arg(name));

    QString newFile = m_unpacker->unpackFile(fileName);
    QFile::remove(fileName);

    if(newFile.isEmpty()) {
        m_state = Stopped;
        emit stateChanged();

        changeStatus(QString("Failed to prepare song %1").arg(name));
        return;
    }

    changeStatus(QString("Caching song %1").arg(name));
    m_cache->cache(newFile);
    doPlay(newFile);
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

Cache * Player::cache() const {
    return m_cache;
}

Downloader * Player::downloader() const {
    return m_downloader;
}

SongModule * Player::currentSong() const {
    return m_module;
}

void Player::doPlay(QString const& fileName) {
    stop();
    if(m_module->load(fileName)) {
        if(m_module->play()) {
            m_state = Playing;
            emit stateChanged();

            changeStatus(QString("Playing %1").arg(m_module->fileName()));
        }
    }
}

void Player::play(QVariant value) {
    qDebug() << "Playing" << value;
    if(value.type() == QVariant::Int) {
        playByModuleId(value.toInt());
    } else if(value.type() == QVariant::String) {
        QString valueString = value.toString();
        bool parseableInt = false;
        int modId = valueString.toInt(&parseableInt, 10);
        if(parseableInt) {
            playByModuleId(modId);
        } else {
            playByModuleFileName(valueString);
        }
    }
}

void Player::playByModuleFileName(QString const& fileName) {
    qDebug() << "Playing module file name=" << fileName;
    if(m_cache->exists(fileName)) {
        doPlay(fileName);
    } else {
        QString name = fileNameOnly(fileName);
        changeStatus(QString("Resolving %1").arg(name));
    }
}

void Player::playByModuleId(int modId) {
    qDebug() << "Playing module id=" << modId;
    changeStatus(QString("Downloading song %1").arg(modId));
    m_downloader->download(modId);
}

void Player::stop() {
    if(state() == Playing) {
        m_state = Stopped;
        emit stateChanged();

        changeStatus("Stopped");
    }
}

void Player::pause() {
    if(state() == Playing) {
        m_state = Paused;
        emit stateChanged();

        changeStatus("Paused");
    }
}

void Player::resume() {
    if(state() == Paused) {
        m_state = Playing;
        emit stateChanged();

        changeStatus("Playing");
    }
}
