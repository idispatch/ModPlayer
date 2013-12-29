#ifndef ALBUM_HPP_
#define ALBUM_HPP_

#include "ItemGroupBase.hpp"
#include "InstanceCounter.hpp"

class Album : public ItemGroupBase,
              public InstanceCounter<Album> {
    Q_OBJECT
    Q_PROPERTY(QString artistName READ artistName NOTIFY artistNameChanged FINAL)
public:
    Album(int id,
          QString const& artistName,
          QString const& name,
          int count,
          QObject *parent);
    ~Album();

    QString const& artistName() const;

    using InstanceCounter<Album>::getInstanceCount;
    using InstanceCounter<Album>::getMaxInstanceCount;
Q_SIGNALS:
    void artistNameChanged();
private:
    Q_DISABLE_COPY(Album)

    QString m_artistName;
};

Q_DECLARE_METATYPE(Album*);

#endif
