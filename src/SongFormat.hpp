#ifndef SONGFORMAT_HPP_
#define SONGFORMAT_HPP_

#include <QDebug>
#include <QMetaType>
#include <QObject>

class SongFormat : public QObject {
    Q_OBJECT

    Q_PROPERTY(int id READ id NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
public:
    SongFormat(int id,
               QString const& name,
               QString const& description,
               int count,
               QObject *parent = 0);
    ~SongFormat();
    int id() const;
    QString const& name() const;
    QString const& description() const;
    int count() const;
Q_SIGNALS:
    void idChanged();
    void nameChanged();
    void descriptionChanged();
    void countChanged();
private:
    Q_DISABLE_COPY(SongFormat)

    int m_id;
    QString m_name;
    QString m_description;
    int m_count;
};

Q_DECLARE_METATYPE(SongFormat*);

QDebug operator << (QDebug dbg, SongFormat const &format);

#endif
