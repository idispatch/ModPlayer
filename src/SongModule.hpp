#ifndef SONGMODULE_HPP_
#define SONGMODULE_HPP_

#include <QMetaType>
#include <QObject>

struct _ModPlugFile;
typedef struct _ModPlugFile ModPlugFile;

class SongModule : public QObject {
    Q_OBJECT

    Q_PROPERTY(bool songLoaded READ songLoaded NOTIFY songLoadedChanged FINAL)

    Q_PROPERTY(QString title READ title WRITE setTitle NOTIFY titleChanged FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged FINAL)

    Q_PROPERTY(int instruments READ instruments WRITE setInstruments NOTIFY instrumentsChanged FINAL)
    Q_PROPERTY(int samples READ samples WRITE setSamples NOTIFY samplesChanged FINAL)
    Q_PROPERTY(int patterns READ patterns WRITE setPatterns NOTIFY patternsChanged FINAL)
    Q_PROPERTY(int channels READ channels WRITE setChannels NOTIFY channelsChanged FINAL)

public:
    SongModule(QObject *parent = 0);
    virtual ~SongModule();

    bool songLoaded() const;

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

    bool load(QString const& fileName);
    void unload();

Q_SIGNALS:
    void songLoadedChanged();

    void titleChanged();
    void descriptionChanged();
    void fileNameChanged();

    void instrumentsChanged();
    void patternsChanged();
    void samplesChanged();
    void channelsChanged();
private:
    static QString fileNameOnly(QString const& fileName);
private:
    QString m_fileFullPath;
    QString m_fileName;
    QString m_title;
    QString m_description;

    int m_instruments;
    int m_channels;
    int m_patterns;
    int m_samples;

    ModPlugFile* m_modPlug;
};

Q_DECLARE_METATYPE(SongModule*);

#endif
