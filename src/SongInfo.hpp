#ifndef SONGINFO_HPP_
#define SONGINFO_HPP_

#include <QMetaType>
#include <QObject>

class SongInfo : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString title READ title WRITE setTitle FINAL)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName FINAL)
    Q_PROPERTY(QString hash READ hash WRITE setHash FINAL)
    Q_PROPERTY(int modId READ modId WRITE setModId FINAL)

public:
    SongInfo(QObject *parent = 0);
    SongInfo(const QString &fileName,
             const QString &title,
             const QString &hash,
             int modId,
             QObject *parent = 0);

    QString title() const;
    QString fileName() const;
    QString hash() const;
    int modId() const;

    void setTitle(const QString &value);
    void setFileName(const QString &value);
    void setHash(const QString &value);
    void setModId(int value);

Q_SIGNALS:
private:
    Q_DISABLE_COPY(SongInfo)

    QString m_fileName;
    QString m_title;
    QString m_hash;
    int m_modId;
};

Q_DECLARE_METATYPE(SongInfo*);

#endif
