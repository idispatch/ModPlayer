#ifndef SONGMODULE_HPP_
#define SONGMODULE_HPP_

#include <QMetaType>
#include <QObject>
#include <QTimer>

struct _ModPlugFile;
typedef struct _ModPlugFile ModPlugFile;

class ModPlayback;

class SongModule : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool songLoaded READ songLoaded NOTIFY songLoadedChanged FINAL)

    Q_PROPERTY(int fileSize READ fileSize WRITE setFileSize NOTIFY fileSizeChanged FINAL)
    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged FINAL)

    Q_PROPERTY(int instruments READ instruments WRITE setInstruments NOTIFY instrumentsChanged FINAL)
    Q_PROPERTY(int samples READ samples WRITE setSamples NOTIFY samplesChanged FINAL)
    Q_PROPERTY(int patterns READ patterns WRITE setPatterns NOTIFY patternsChanged FINAL)
    Q_PROPERTY(int channels READ channels WRITE setChannels NOTIFY channelsChanged FINAL)

    Q_PROPERTY(int currentOrder READ currentOrder WRITE setCurrentOrder NOTIFY currentOrderChanged FINAL)
    Q_PROPERTY(int currentPattern READ currentPattern WRITE setCurrentPattern NOTIFY currentPatternChanged FINAL)
    Q_PROPERTY(int currentRow READ currentRow WRITE setCurrentRow NOTIFY currentRowChanged FINAL)
    Q_PROPERTY(int currentSpeed READ currentSpeed WRITE setCurrentSpeed NOTIFY currentSpeedChanged FINAL)
    Q_PROPERTY(int currentTempo READ currentTempo WRITE setCurrentTempo NOTIFY currentTempoChanged FINAL)

    Q_PROPERTY(int masterVolume READ masterVolume WRITE setMasterVolume NOTIFY masterVolumeChanged FINAL)
    Q_PROPERTY(int playingChannels READ playingChannels WRITE setPlayingChannels NOTIFY playingChannelsChanged FINAL)

    Q_PROPERTY(int length READ length WRITE setLength NOTIFY lengthChanged FINAL)
    Q_PROPERTY(QString lengthTimeString READ lengthTimeString NOTIFY lengthTimeStringChanged FINAL)

public:
    SongModule(QObject *parent = 0);
    virtual ~SongModule();

    bool songLoaded() const;

    int fileSize() const;
    void setFileSize(int value);

    QString title() const;
    void setTitle(const QString &value);

    QString description() const;
    void setDescription(const QString &value);

    QString fileName() const;
    void setFileName(const QString &value);

    int instruments() const;
    void setInstruments(int value);

    int samples() const;
    void setSamples(int value);

    int patterns() const;
    void setPatterns(int value);

    int channels() const;
    void setChannels(int value);

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

    int length() const;
    QString lengthTimeString() const;
    void setLength(int value);

    bool load(QString const& fileName);
    Q_INVOKABLE bool play();
    Q_INVOKABLE bool stop();
    Q_INVOKABLE bool pause();
    Q_INVOKABLE bool resume();
    void unload();

Q_SIGNALS:
    void songLoadedChanged();

    void fileSizeChanged();
    void titleChanged();
    void descriptionChanged();
    void fileNameChanged();

    void instrumentsChanged();
    void patternsChanged();
    void samplesChanged();
    void channelsChanged();

    void currentOrderChanged();
    void currentPatternChanged();
    void currentRowChanged();
    void currentSpeedChanged();
    void currentTempoChanged();
    void masterVolumeChanged();
    void playingChannelsChanged();
    void lengthChanged();
    void lengthTimeStringChanged();

private slots:
    void onUpdateTimeout();
private:
    Q_DISABLE_COPY(SongModule)

    static QString fileNameOnly(QString const& fileName);

    void startRefreshTimer();
    void stopRefreshTimer();
private:
    int m_fileSize;

    QString m_fileFullPath;
    QString m_fileName;
    QString m_title;
    QString m_description;

    int m_instruments;
    int m_channels;
    int m_patterns;
    int m_samples;

    int m_currentOrder;
    int m_currentPattern;
    int m_currentRow;
    int m_currentSpeed;
    int m_currentTempo;
    int m_masterVolume;
    int m_playingChannels;
    int m_length;

    ModPlugFile* m_modPlug;
    ModPlayback * m_playback;
    QTimer * m_timer;
};

Q_DECLARE_METATYPE(SongModule*);

#endif
