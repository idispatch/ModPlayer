#include "SongModule.hpp"
#include "SongFormat.hpp"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QByteArray>

template<>
int InstanceCounter<SongModule>::s_count;
template<>
int InstanceCounter<SongModule>::s_maxCount;

using namespace bb::cascades;

SongModule::SongModule(QObject *parent) :
    SongExtendedInfo(parent),
    m_currentOrder(0),
    m_currentPattern(0),
    m_currentRow(0),
    m_currentSpeed(0),
    m_currentTempo(0),
    m_masterVolume(0),
    m_playingChannels(0),
    m_modPlug(NULL) {
    memset(&m_channelVU[0], 0, sizeof(m_channelVU));
}

SongModule::~SongModule() {
    if (m_modPlug != NULL) {
        ModPlug_Unload(m_modPlug);
        m_modPlug = NULL;
    }
}

bool SongModule::songLoaded() const {
    return m_modPlug != NULL;
}

QString SongModule::absoluteFileName() const {
    return m_absoluteFileName;
}

QString SongModule::description() const {
    return m_description;
}

void SongModule::setDescription(const QString &value) {
    QString description = value.trimmed();
    if (m_description != description) {
        m_description = description;
        emit descriptionChanged();
    }
}

int SongModule::currentOrder() const {
    return m_currentOrder;
}

void SongModule::setCurrentOrder(int value) {
    if (m_currentOrder != value) {
        m_currentOrder = value;
        emit currentOrderChanged();
    }
}

int SongModule::currentPattern() const {
    return m_currentPattern;
}

void SongModule::setCurrentPattern(int value) {
    if (m_currentPattern != value) {
        m_currentPattern = value;
        emit currentPatternChanged();
    }
}

int SongModule::currentRow() const {
    return m_currentRow;
}

void SongModule::setCurrentRow(int value) {
    if (m_currentRow != value) {
        m_currentRow = value;
        emit currentRowChanged();
    }
}

int SongModule::currentSpeed() const {
    return m_currentSpeed;
}

void SongModule::setCurrentSpeed(int value) {
    if (m_currentSpeed != value) {
        m_currentSpeed = value;
        emit currentSpeedChanged();
    }
}

int SongModule::currentTempo() const {
    return m_currentTempo;
}

void SongModule::setCurrentTempo(int value) {
    if (m_currentTempo != value) {
        m_currentTempo = value;
        emit currentTempoChanged();
    }
}

int SongModule::playingChannels() const {
    return m_playingChannels;
}

void SongModule::setPlayingChannels(int value) {
    if (m_playingChannels != value) {
        m_playingChannels = value;
        emit playingChannelsChanged();
    }
}

int SongModule::masterVolume() const {
    return m_masterVolume;
}

void SongModule::setMasterVolume(int value) {
    if (m_masterVolume != value) {
        m_masterVolume = value;
        emit masterVolumeChanged();
    }
}

bool SongModule::load(SongExtendedInfo const& info, QString const& fileName) {
    if (m_absoluteFileName == fileName) {
        return true; // not loading the same song twice
    }

    bool songWasLoaded = false;
    if (m_modPlug != NULL) {
        songWasLoaded = true;
        ModPlug_Unload(m_modPlug);
        m_modPlug = NULL;
    }
    m_absoluteFileName = ""; // no song loaded
    emit absoluteFileNameChanged();

    QFile fileIn(fileName);
    if(fileIn.open(QFile::ReadOnly)) {
        QByteArray data = fileIn.readAll();
        m_modPlug = ModPlug_Load(data.data(), data.size());
        if (m_modPlug != NULL) {
            m_absoluteFileName = fileName;
            emit absoluteFileNameChanged();

            setFileName(fileName);
            setTitle(ModPlug_GetName(m_modPlug));

            assignInfo(info);

            const char * description = ModPlug_GetMessage(m_modPlug);
            setDescription(description ? description : "");

            setInstruments(ModPlug_NumInstruments(m_modPlug));
            setChannels(ModPlug_NumChannels(m_modPlug));
            setSamples(ModPlug_NumSamples(m_modPlug));
            setPatterns(ModPlug_NumPatterns(m_modPlug));
            setOrders(ModPlug_NumOrders(m_modPlug));

            setFileSize(data.size());
            setSongLength(ModPlug_GetLength(m_modPlug));

            update();

            emit songLoadedChanged();
        }
        else
        {
            unload();
        }
    }
    else
    {
        qDebug() << "Could not open" << fileName;
        if (songWasLoaded)
        {
            emit songLoadedChanged();
        }
    }
    return songLoaded();
}

void SongModule::save(QString const& fileName) {
    const QString originalFileName = absoluteFileName();
    bool copyOk;
    if(QFile::exists(fileName))
    {
        if(QFile::remove(fileName)) {
            copyOk = true;
        } else {
            copyOk = false;
        }
    } else {
        copyOk = true;
    }
    if(copyOk) {
        QFile::copy(originalFileName, fileName);
    }
}

bool SongModule::unload() {
    if (m_modPlug != NULL)
    {
        ModPlug_Unload(m_modPlug);
        m_modPlug = NULL;

        m_absoluteFileName = ""; // no song loaded
        emit absoluteFileNameChanged();

        setFileName("");
        setTitle("");
        setDescription("");

        setInstruments(0);
        setChannels(0);
        setSamples(0);
        setPatterns(0);
        setOrders(0);

        setFileSize(0);

        update(true);

        emit songLoadedChanged();
    }
    return true;
}

bool SongModule::rewind() {
    if (m_modPlug != NULL) {
        ModPlug_Seek(m_modPlug, 0);
        update();
        return true;
    }
    return false;
}

void SongModule::seekToOrder(int order) {
    if (m_modPlug != NULL) {
        if(order != currentOrder()) {
            ModPlug_SeekOrder(m_modPlug, order);
            update();
        }
    }
}

ArrayDataModel* SongModule::getSampleNames() {
    ArrayDataModel * model = new ArrayDataModel();
    unsigned sampleCount = ModPlug_NumSamples(m_modPlug);
    unsigned int i = 0;
    while (sampleCount-- > 0) {
        char buffer[64];
        ModPlug_SampleName(m_modPlug, i++, buffer);
        QVariantMap sampleInfo;
        sampleInfo["id"] = i;
        sampleInfo["name"] = QVariant::fromValue(QString(buffer));
        model->append(QVariant::fromValue(sampleInfo));
    }
    return model;
}

ArrayDataModel* SongModule::getInstrumentNames() {
    ArrayDataModel * model = new ArrayDataModel();
    unsigned sampleCount = ModPlug_NumInstruments(m_modPlug);
    unsigned int i = 0;
    while (sampleCount-- > 0) {
        char buffer[64];
        ModPlug_InstrumentName(m_modPlug, i++, buffer);
        QVariantMap instrumentInfo;
        instrumentInfo["id"] = i;
        instrumentInfo["name"] = QVariant::fromValue(QString(buffer));
        model->append(QVariant::fromValue(instrumentInfo));
    }
    return model;
}

void SongModule::assignInfo(SongExtendedInfo const& other) {
    setId(other.id());
    setFormatId(other.formatId());
    if(formatId() == 0) {
        setFormatId(SongFormat::getFormatIdByFileName(fileName()));
    }
    setFileSize(other.fileSize());
    setSongLength(other.songLength());

    setDownloads(other.downloads());
    setFavourited(other.favourited());
    setScore(other.score());
    setPlayCount(other.playCount());
    setLastPlayed(other.lastPlayed());
    setMyFavourite(other.myFavourite());

    setFormat(other.format());
    setTracker(other.tracker());
    setGenre(other.genre());
    setArtistId(other.artistId());
    setArtist(other.artist());
}

void SongModule::update(bool endOfSong) {
    setCurrentOrder(m_modPlug != NULL && !endOfSong ? ModPlug_GetCurrentOrder(m_modPlug) : 0);
    setCurrentPattern(m_modPlug != NULL && !endOfSong ? ModPlug_GetCurrentPattern(m_modPlug) : 0);
    setCurrentRow(m_modPlug != NULL && !endOfSong ? ModPlug_GetCurrentRow(m_modPlug) : 0);
    setCurrentSpeed(m_modPlug != NULL && !endOfSong ? ModPlug_GetCurrentSpeed(m_modPlug) : 0);
    setCurrentTempo(m_modPlug != NULL && !endOfSong ? ModPlug_GetCurrentTempo(m_modPlug) : 0);
    setMasterVolume(m_modPlug != NULL && !endOfSong ? ModPlug_GetMasterVolume(m_modPlug) : 0);
    setPlayingChannels(m_modPlug != NULL && !endOfSong ? ModPlug_GetPlayingChannels(m_modPlug) : 0);
    setSongLength(m_modPlug != NULL && !endOfSong ? ModPlug_GetLength(m_modPlug) : 0);

    updateChannelVU(endOfSong);
}

void SongModule::updateChannelVU(bool endOfSong) {
    bool bChanged = false;
    if(m_modPlug != NULL)
    {
        if(endOfSong)
        {
            // Set all channel VU to 0
            memset(&m_channelVU[0], 0, sizeof(m_channelVU));
            bChanged = true;
        }
        else
        {
            // Update all channel VU values
            const int numChannels = channels();
            unsigned result[128];
            ModPlug_GetChannelVUs(m_modPlug, 0, numChannels, result);
            const size_t numBytes = sizeof(unsigned) * numChannels;
            if(memcmp(m_channelVU, result, numBytes))
            {
                memcpy(m_channelVU, result, numBytes);
                bChanged = true;
            }
        }
    }
    if(bChanged)
    {
        emit channelVUChanged();
    }
}

int SongModule::getChannelVU(int channel) {
    return m_channelVU[channel];
}

ModPlugNote* SongModule::getPattern(int pattern, int* numrows) {
    ModPlugNote* result = NULL;
    if(m_modPlug != 0) {
        result = ModPlug_GetPattern(m_modPlug,
                                    pattern,
                                    reinterpret_cast<unsigned int*>(numrows));
    }
    return result;
}

void SongModule::muteChannel(int channel, bool mute) {
    ModPlug_MuteChannel(m_modPlug, channel, mute);
}

bool SongModule::isChannelMuted(int channel) {
    return ModPlug_IsChannelMuted(m_modPlug, channel);
}

SongModule::operator ModPlugFile*() {
    return m_modPlug;
}
