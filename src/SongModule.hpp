#ifndef SONGMODULE_HPP_
#define SONGMODULE_HPP_

#include <QMetaType>
#include <QObject>
#include <vector>
#include  <bb/cascades/ArrayDataModel>
#include "SongExtendedInfo.hpp"
#include "InstanceCounter.hpp"

struct _ModPlugFile;
typedef struct _ModPlugFile ModPlugFile;

class SongModule : public SongExtendedInfo,
                   public InstanceCounter<SongModule> {
    Q_OBJECT

    Q_PROPERTY(bool songLoaded READ songLoaded NOTIFY songLoadedChanged FINAL)

    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString absoluteFileName READ absoluteFileName NOTIFY absoluteFileNameChanged FINAL)

    Q_PROPERTY(int currentOrder READ currentOrder WRITE setCurrentOrder NOTIFY currentOrderChanged FINAL)
    Q_PROPERTY(int currentPattern READ currentPattern WRITE setCurrentPattern NOTIFY currentPatternChanged FINAL)
    Q_PROPERTY(int currentRow READ currentRow WRITE setCurrentRow NOTIFY currentRowChanged FINAL)
    Q_PROPERTY(int currentSpeed READ currentSpeed WRITE setCurrentSpeed NOTIFY currentSpeedChanged FINAL)
    Q_PROPERTY(int currentTempo READ currentTempo WRITE setCurrentTempo NOTIFY currentTempoChanged FINAL)

    Q_PROPERTY(int masterVolume READ masterVolume WRITE setMasterVolume NOTIFY masterVolumeChanged FINAL)
    Q_PROPERTY(int playingChannels READ playingChannels WRITE setPlayingChannels NOTIFY playingChannelsChanged FINAL)
public:
    SongModule(QObject *parent);
    virtual ~SongModule();

    bool songLoaded() const;

    QString description() const;
    void setDescription(const QString &value);

    QString absoluteFileName() const;

    int currentOrder() const;
    void setCurrentOrder(int value);

    int currentPattern() const;
    void setCurrentPattern(int value);

    int currentRow() const;
    void setCurrentRow(int value);

    int currentSpeed() const;
    void setCurrentSpeed(int value);

    int currentTempo() const;
    void setCurrentTempo(int value);

    int masterVolume() const;
    void setMasterVolume(int value);

    int playingChannels() const;
    void setPlayingChannels(int value);

    Q_INVOKABLE void update(bool endOfSong = false);
    Q_INVOKABLE bool load(SongExtendedInfo const& info, QString const& fileName);
    Q_INVOKABLE void save(QString const& fileName);
    Q_INVOKABLE bool unload();
    Q_INVOKABLE bool rewind();

    Q_INVOKABLE int getChannelVU(int channel);

    Q_INVOKABLE bb::cascades::ArrayDataModel* getSampleNames();
    Q_INVOKABLE bb::cascades::ArrayDataModel* getInstrumentNames();

    operator ModPlugFile* ();

    using InstanceCounter<SongModule>::getInstanceCount;
    using InstanceCounter<SongModule>::getMaxInstanceCount;
Q_SIGNALS:
    void songLoadedChanged();

    void descriptionChanged();
    void absoluteFileNameChanged();
    void currentOrderChanged();
    void currentPatternChanged();
    void currentRowChanged();
    void currentSpeedChanged();
    void currentTempoChanged();
    void masterVolumeChanged();
    void playingChannelsChanged();

    void channelVUChanged(int channel, int channelVU);

private slots:
private:
    Q_DISABLE_COPY(SongModule)

    void assignInfo(SongExtendedInfo const& other);
    void updateChannelVU(bool endOfSong);
private:
    QString m_absoluteFileName;
    QString m_description;

    int m_currentOrder;
    int m_currentPattern;
    int m_currentRow;
    int m_currentSpeed;
    int m_currentTempo;

    int m_masterVolume;
    int m_playingChannels;

    std::vector<unsigned> m_channelVU;

    ModPlugFile* m_modPlug;
};

Q_DECLARE_METATYPE(SongModule*);

#endif
