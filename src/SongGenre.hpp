#ifndef SONGGENRE_HPP_
#define SONGGENRE_HPP_

#include <QMetaType>
#include <QObject>

class SongGenre : public QObject {
    Q_OBJECT

    Q_PROPERTY(int id READ id NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
public:
    SongGenre(int id,
              QString const& name,
              int count,
              QObject *parent = 0);
    ~SongGenre();
    int id() const;
    QString const& name() const;
    int count() const;
Q_SIGNALS:
    void idChanged();
    void nameChanged();
    void countChanged();
private:
    Q_DISABLE_COPY(SongGenre)

    int m_id;
    QString m_name;
    int m_count;
};

Q_DECLARE_METATYPE(SongGenre*);

#endif
