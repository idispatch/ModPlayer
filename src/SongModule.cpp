#include "modplug.h"

#include "SongModule.hpp"
#include "ModPlayback.hpp"

#include <QFile>
#include <QFileInfo>
#include <QByteArray>

SongModule::SongModule(QObject *parent)
 : QObject(parent),
   m_instruments(0),
   m_channels(0),
   m_patterns(0),
   m_samples(0),
   m_modPlug(NULL),
   m_playback(NULL) {
}

SongModule::~SongModule() {
    if(m_playback!=NULL) {
        //m_playback
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

            setInstruments(ModPlug_NumInstruments(m_modPlug));
            setChannels(ModPlug_NumChannels(m_modPlug));
            setSamples(ModPlug_NumSamples(m_modPlug));
            setPatterns(ModPlug_NumPatterns(m_modPlug));

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
        m_fileFullPath.clear();
        ModPlug_Unload(m_modPlug);
        m_modPlug = NULL;
        emit songLoadedChanged();
        setFileName("");
        setTitle("");
        setDescription("");
        setInstruments(0);
        setChannels(0);
        setSamples(0);
        setPatterns(0);
    }
}
