#include "SongModule.hpp"
#include "FileUtils.hpp"
#include <QDebug>
#include <QFile>
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
    m_masterVolume(128),
    m_playingChannels(0),
    m_modPlug(NULL) {
    memset(&m_channelVU[0], 0, sizeof(m_channelVU));
}

SongModule::~SongModule() {
    if (m_modPlug != NULL) {
        ::ModPlug_Unload(m_modPlug);
        m_modPlug = NULL;
    }
#ifdef VERBOSE_LOGGING
    qDebug() << "SongModule::~SongModule()";
#endif
}

void SongModule::reset() {
    setCurrentOrder(0);
    setCurrentPattern(0);
    setCurrentRow(0);
    setCurrentSpeed(0);
    setCurrentTempo(0);
    setPlayingChannels(0);

    setChannels(0);
    setInstruments(0);
    setChannels(0);
    setSamples(0);
    setPatterns(0);
    setOrders(0);

    setArtist("");
    setArtistId(0);

    setFormat("");

    setTracker("");
    setTrackerId(0);

    setGenre("");
    setGenreId(0);

    setAbsoluteFileName("");
    setDescription("");

    memset(&m_channelVU[0], 0, sizeof(m_channelVU));
}

bool SongModule::songLoaded() const {
    if(m_modPlug != NULL)
        return true;
    return m_absoluteFileName.length() > 0 && !isTrackerSong();
}

QString const& SongModule::absoluteFileName() const {
    return m_absoluteFileName;
}

QString const& SongModule::description() const {
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
    if(value < 1) {
        value = 1;
    }
    if(value > 512) {
        value = 512;
    }
    if (m_masterVolume != value) {
        m_masterVolume = value;
        if (m_modPlug != NULL) {
            ::ModPlug_SetMasterVolume(m_modPlug, m_masterVolume);
        }
        emit masterVolumeChanged();
    }
}

bool SongModule::load(SongExtendedInfo const& info, QString const& fileName) {
    if (m_absoluteFileName == fileName) {
        return true; // not loading the same song twice
    }

    const bool songWasLoaded = m_modPlug != NULL || m_absoluteFileName.length() > 0;
    if (m_modPlug != NULL) {
        ::ModPlug_Unload(m_modPlug);
        m_modPlug = NULL;
    }

    reset();
    if(SongFormat::isTrackerSong(fileName)) {
        QFile fileIn(fileName);
        if(fileIn.open(QFile::ReadOnly)) {
            QByteArray data = fileIn.readAll();
            m_modPlug = ::ModPlug_Load(data.data(), data.size());
            if (m_modPlug != NULL) {
                setAbsoluteFileName(fileName);

                assignInfo(info);

                setFileName(fileName);
                setTitle(::ModPlug_GetName(m_modPlug));

                const char * description = ::ModPlug_GetMessage(m_modPlug);
                setDescription(description ? description : "");

                setInstruments(::ModPlug_NumInstruments(m_modPlug));
                setChannels(::ModPlug_NumChannels(m_modPlug));
                setSamples(::ModPlug_NumSamples(m_modPlug));
                setPatterns(::ModPlug_NumPatterns(m_modPlug));
                setOrders(::ModPlug_NumOrders(m_modPlug));

                setFileSize(data.size());
                setSongLength(::ModPlug_GetLength(m_modPlug));

                update();

                emit songLoadedChanged();
                emit isTrackerSongChanged();
            } else {
                qDebug() << "Failed to load ModPlug file" << fileName;
                unload();
            }
        } else {
            qDebug() << "Could not open tracker song" << fileName;
            if(songWasLoaded) {
                emit songLoadedChanged();
            }
        }
    } else {
        setAbsoluteFileName(fileName);
        setFileName(fileName);
        setDescription("");

        setCurrentOrder(0);
        setCurrentPattern(0);
        setCurrentRow(0);
        setCurrentSpeed(0);
        setCurrentTempo(0);
        setChannels(0);

        setInstruments(0);
        setChannels(0);
        setSamples(0);
        setPatterns(0);
        setOrders(0);

        assignInfo(info);

        setCurrentOrder(0);
        setOrders(info.songLength()); // song length in milliseconds
        update();

        emit songLoadedChanged();
        emit isTrackerSongChanged();
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
            qDebug() << "Failed to remove file " << fileName;
            copyOk = false;
        }
    } else {
        copyOk = true;
    }
    if(copyOk) {
        if(QFile::copy(originalFileName, fileName) == true) {
            if(!FileUtils::adjustPermissions(fileName)) {
                qDebug() << "Failed to set permissions for file " << fileName;
            }
        }
    }
}

bool SongModule::unload() {
    bool songWasLoaded = songLoaded();
    if (m_modPlug != NULL) {
        ::ModPlug_Unload(m_modPlug);
        m_modPlug = NULL;
    }

    reset();

    update(true);

    if(songWasLoaded) {
        emit songLoadedChanged();
    }

    return true;
}

bool SongModule::rewind() {
    if (m_modPlug != NULL) {
        ::ModPlug_SeekOrder(m_modPlug, 0);
        update();
        return true;
    }
    return false;
}

bool SongModule::seekToOrder(int order) {
    if (m_modPlug != NULL) {
        if(order != currentOrder()) {
            ::ModPlug_SeekOrder(m_modPlug, order);
            update();
        }
        return true;
    }
    return false;
}

ArrayDataModel* SongModule::getSampleNames() {
    ArrayDataModel * model = new ArrayDataModel();
    if(m_modPlug != NULL) {
        unsigned sampleCount = ::ModPlug_NumSamples(m_modPlug);
        unsigned int i = 0;
        while (sampleCount-- > 0) {
            char buffer[64];
            ::ModPlug_SampleName(m_modPlug, i++, buffer);
            QVariantMap sampleInfo;
            sampleInfo["id"] = i;
            sampleInfo["name"] = QVariant::fromValue(QString(buffer));
            model->append(QVariant::fromValue(sampleInfo));
        }
    }
    return model;
}

ArrayDataModel* SongModule::getInstrumentNames() {
    ArrayDataModel * model = new ArrayDataModel();
    if(m_modPlug != NULL) {
        unsigned sampleCount = ::ModPlug_NumInstruments(m_modPlug);
        unsigned int i = 0;
        while (sampleCount-- > 0) {
            char buffer[64];
            ::ModPlug_InstrumentName(m_modPlug, i++, buffer);
            QVariantMap instrumentInfo;
            instrumentInfo["id"] = i;
            instrumentInfo["name"] = QVariant::fromValue(QString(buffer));
            model->append(QVariant::fromValue(instrumentInfo));
        }
    }
    return model;
}

void SongModule::setAbsoluteFileName(QString const& fileName) {
    if(fileName != m_absoluteFileName) {
        m_absoluteFileName = fileName;
        emit absoluteFileNameChanged();
    }
}

void SongModule::assignInfo(SongExtendedInfo const& other) {
    setId(other.id());
    setTitle(other.title());
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
    setGenreId(other.genreId());
    setArtistId(other.artistId());
    setArtist(other.artist());
}

void SongModule::update(bool endOfSong) {
    if(m_modPlug != NULL && !endOfSong) {
        setCurrentOrder(::ModPlug_GetCurrentOrder(m_modPlug));
        setCurrentPattern(::ModPlug_GetCurrentPattern(m_modPlug));
        setCurrentRow(::ModPlug_GetCurrentRow(m_modPlug));
        setCurrentSpeed(::ModPlug_GetCurrentSpeed(m_modPlug));
        setCurrentTempo(::ModPlug_GetCurrentTempo(m_modPlug));
        setPlayingChannels(::ModPlug_GetPlayingChannels(m_modPlug));
        setSongLength(::ModPlug_GetLength(m_modPlug));
    } else {
        if(endOfSong) {
            setCurrentOrder(0);
            setCurrentPattern(0);
            setCurrentRow(0);
            setCurrentSpeed(0);
            setCurrentTempo(0);
            setPlayingChannels(0);
        }
    }

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
            unsigned result[sizeof(m_channelVU) / sizeof(m_channelVU[0])];
            ::ModPlug_GetChannelVUs(m_modPlug, 0, numChannels, result);
            const size_t numBytes = sizeof(unsigned) * numChannels;
            if(memcmp(m_channelVU, result, numBytes))
            {
                memcpy(m_channelVU, result, numBytes);
                bChanged = true;
            }
        }
    }
    if(bChanged) {
        emit channelVUChanged();
    }
}

int SongModule::getChannelVU(int channel) {
    if(m_modPlug != NULL)
    {
        if(channel >= 0 && channel < static_cast<int>(sizeof(m_channelVU)/sizeof(m_channelVU[0])))
        {
            return m_channelVU[channel];
        }
    }
    return 0;
}

ModPlugNote* SongModule::getPattern(int pattern, int* numrows) {
    ModPlugNote* result = NULL;
    if(m_modPlug != NULL) {
        result = ::ModPlug_GetPattern(m_modPlug,
                                      pattern,
                                      reinterpret_cast<unsigned int*>(numrows));
    }
    return result;
}

void SongModule::muteChannel(int channel, bool mute) {
    if(m_modPlug != NULL) {
        ::ModPlug_MuteChannel(m_modPlug, channel, mute);
    }
}

bool SongModule::isChannelMuted(int channel) {
    if(m_modPlug != NULL) {
        return ::ModPlug_IsChannelMuted(m_modPlug, channel);
    }
    return false;
}

SongModule::operator ModPlugFile*() {
    return m_modPlug;
}
