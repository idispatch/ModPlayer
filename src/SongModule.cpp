#include "modplug.h"

#include "SongModule.hpp"
#include "ModPlayback.hpp"

#include <QFile>
#include <QFileInfo>
#include <QByteArray>

SongModule::SongModule(QObject *parent)
 : QObject(parent),
   m_fileSize(0),
   m_instruments(0),
   m_channels(0),
   m_patterns(0),
   m_samples(0),
   m_currentOrder(0),
   m_currentPattern(0),
   m_currentRow(0),
   m_currentSpeed(0),
   m_currentTempo(0),
   m_masterVolume(0),
   m_playingChannels(0),
   m_length(0),
   m_modPlug(NULL),
   m_playback(NULL),
   m_timer(new QTimer(this)) {
    QTimer::connect(m_timer,
                    SIGNAL(timeout()),
                    this,
                    SLOT(onUpdateTimeout()));
}

SongModule::~SongModule() {
    if(m_timer != NULL) {
        m_timer->stop();
    }
    if(m_playback!=NULL) {
        m_playback->stopThread();
        m_playback->wait();
    }
    if(m_modPlug!=NULL) {
        ModPlug_Unload(m_modPlug);
        m_modPlug = NULL;
    }
}

bool SongModule::songLoaded() const {
    return m_modPlug != NULL;
}

QString SongModule::fileName() const {
    return m_fileName;
}

void SongModule::setFileName(const QString &value) {
    if(m_fileName != value) {
        m_fileName = value;
        emit fileNameChanged();
    }
}

QString SongModule::title() const {
    return m_title;
}

void SongModule::setTitle(const QString &value) {
    if(m_title != value) {
        m_title = value;
        emit titleChanged();
    }
}

QString SongModule::description() const {
    return m_description;
}

void SongModule::setDescription(const QString &value) {
    if(m_description != value) {
        m_description = value;
        emit descriptionChanged();
    }
}

int SongModule::instruments() const {
    return m_modPlug != NULL ? ModPlug_NumInstruments(m_modPlug) : 0;
}

void SongModule::setInstruments(int value) {
    if(m_instruments != value) {
        m_instruments = value;
        emit instrumentsChanged();
    }
}

int SongModule::samples() const {
    return m_modPlug != NULL ? ModPlug_NumSamples(m_modPlug) : 0;
}

void SongModule::setSamples(int value) {
    if(m_samples != value) {
        m_samples = value;
        emit samplesChanged();
    }
}

int SongModule::channels() const {
    return m_modPlug != NULL ? ModPlug_NumChannels(m_modPlug) : 0;
}

void SongModule::setChannels(int value) {
    if(m_channels != value) {
        m_channels = value;
        emit channelsChanged();
    }
}

int SongModule::patterns() const {
    return m_modPlug != NULL ? ModPlug_NumPatterns(m_modPlug) : 0;
}

void SongModule::setPatterns(int value) {
    if(m_patterns != value) {
        m_patterns = value;
        emit patternsChanged();
    }
}

int SongModule::currentOrder() const {
    return m_modPlug != NULL ? ModPlug_GetCurrentOrder(m_modPlug) : 0;
}

void SongModule::setCurrentOrder(int value) {
    if(m_currentOrder != value) {
        m_currentOrder = value;
        emit currentOrderChanged();
    }
}

int SongModule::currentPattern() const {
    return m_modPlug != NULL ? ModPlug_GetCurrentPattern(m_modPlug) : 0;
}

void SongModule::setCurrentPattern(int value) {
    if(m_currentPattern != value) {
        m_currentPattern = value;
        emit currentPatternChanged();
    }
}

int SongModule::currentRow() const {
    return m_modPlug != NULL ? ModPlug_GetCurrentRow(m_modPlug) : 0;
}

void SongModule::setCurrentRow(int value) {
    if(m_currentRow != value) {
        m_currentRow = value;
        emit currentRowChanged();
    }
}

int SongModule::currentSpeed() const {
    return m_modPlug != NULL ? ModPlug_GetCurrentSpeed(m_modPlug) : 0;
}

void SongModule::setCurrentSpeed(int value) {
    if(m_currentSpeed != value) {
        m_currentSpeed = value;
        emit currentSpeedChanged();
    }
}

int SongModule::currentTempo() const {
    return m_modPlug != NULL ? ModPlug_GetCurrentTempo(m_modPlug) : 0;
}

void SongModule::setCurrentTempo(int value) {
    if(m_currentTempo != value) {
        m_currentTempo = value;
        emit currentTempoChanged();
    }
}

QString SongModule::lengthTimeString() const {
    int seconds = 0;
    int minutes = 0;
    if(m_modPlug != NULL) {
        int length = ModPlug_GetLength(m_modPlug);
        int totalSeconds = length / 1000;
        seconds = totalSeconds % 60;
        minutes = totalSeconds / 60;
    }
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0'))
                           .arg(seconds, 2, 10, QChar('0'));
}

int SongModule::length() const {
    return m_modPlug != NULL ? ModPlug_GetLength(m_modPlug) : 0;
}

void SongModule::setLength(int value) {
    if(m_length != value) {
        m_length = value;
        emit lengthChanged();
    }
}

int SongModule::playingChannels() const {
    return m_modPlug != NULL ? ModPlug_GetPlayingChannels(m_modPlug) : 0;
}

void SongModule::setPlayingChannels(int value) {
    if(m_playingChannels != value) {
        m_playingChannels = value;
        emit playingChannelsChanged();
    }
}

int SongModule::masterVolume() const {
    return m_modPlug != NULL ? ModPlug_GetMasterVolume(m_modPlug) : 0;
}

void SongModule::setMasterVolume(int value) {
    if(m_masterVolume != value) {
        m_masterVolume = value;
        emit masterVolumeChanged();
    }
}

int SongModule::fileSize() const {
    return m_fileSize;
}

void SongModule::setFileSize(int value) {
    if(m_fileSize != value) {
        m_fileSize = value;
        emit fileSizeChanged();
    }
}

QString SongModule::fileNameOnly(QString const& fileName) {
    QFile file(fileName);
    QFileInfo fileInfo(file.fileName());
    return fileInfo.fileName();
}

bool SongModule::play() {
    if(songLoaded()) {
        if(m_playback == NULL || m_playback->isFinished()) {
            m_playback = new ModPlayback(m_modPlug, this);
            m_playback->start(QThread::NormalPriority);

            m_timer->start(100);
        }
        return true;
    } else {
        return false;
    }
}

bool SongModule::stop() {
    if(songLoaded()) {
        if(m_timer->isActive()) {
            m_timer->stop();
        }

        if(m_playback != NULL) {
            m_playback->stopThread();
            m_playback->wait();
        }

        return true;
    } else {
        return false;
    }
}

bool SongModule::load(QString const& fileName) {
    if(fileName == m_fileFullPath) {
        return true;
    }

    unload();

    QFile fileIn (fileName);
    if(fileIn.open(QFile::ReadOnly)) {
        QByteArray data = fileIn.readAll();
        m_modPlug = ModPlug_Load(data.data(), data.size());
        if(m_modPlug != NULL) {
            m_fileFullPath = fileName;

            setFileName(fileNameOnly(fileName));
            setTitle(ModPlug_GetName(m_modPlug));

            const char * description = ModPlug_GetMessage(m_modPlug);
            setDescription(description ? description : "");

            setInstruments(instruments());
            setChannels(channels());
            setSamples(samples());
            setPatterns(patterns());

            setFileSize(data.size());

            setCurrentOrder(currentOrder());
            setCurrentPattern(currentPattern());
            setCurrentRow(currentRow());
            setCurrentSpeed(currentSpeed());
            setCurrentTempo(currentTempo());
            setMasterVolume(masterVolume());
            setPlayingChannels(playingChannels());
            setLength(length());

            emit songLoadedChanged();

            return true;
        } else {
            unload();
        }
    }
    return false;
}

void SongModule::unload() {
    if(m_modPlug!=NULL) {

        if(m_playback!=NULL && m_playback->isRunning()) {
            m_playback->stopThread();
            m_playback->wait();
        }

        if(m_timer->isActive()) {
            m_timer->stop();
        }

        ModPlug_Unload(m_modPlug);
        m_modPlug = NULL;

        m_fileFullPath.clear();
        setFileName("");
        setTitle("");
        setDescription("");

        setInstruments(0);
        setChannels(0);
        setSamples(0);
        setPatterns(0);

        setFileSize(0);

        setCurrentOrder(0);
        setCurrentPattern(0);
        setCurrentRow(0);
        setCurrentSpeed(0);
        setCurrentTempo(0);
        setMasterVolume(0);
        setPlayingChannels(0);
        setLength(0);

        emit songLoadedChanged();
    }
}

void SongModule::onUpdateTimeout() {
    setCurrentOrder(currentOrder());
    setCurrentPattern(currentPattern());
    setCurrentRow(currentRow());
    setCurrentSpeed(currentSpeed());
    setCurrentTempo(currentTempo());
    setMasterVolume(masterVolume());
    setPlayingChannels(playingChannels());
}
