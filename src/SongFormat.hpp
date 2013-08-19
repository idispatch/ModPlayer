#ifndef SONGFORMAT_HPP_
#define SONGFORMAT_HPP_

#include <QUrl>
#include "ItemGroupBase.hpp"
#include "InstanceCounter.hpp"

class SongFormat : public ItemGroupBase,
                   public InstanceCounter<SongFormat> {
    Q_OBJECT
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(QUrl iconPath READ iconPath NOTIFY iconPathChanged FINAL)
public:
    SongFormat(int id,
               QString const& name,
               QString const& description,
               int count,
               QObject *parent = 0);
    ~SongFormat();
    QString const& description() const;
    QUrl iconPath() const;

    static QUrl getIconPath(int formatId);

    using InstanceCounter<SongFormat>::getInstanceCount;
    using InstanceCounter<SongFormat>::getMaxInstanceCount;
Q_SIGNALS:
    void descriptionChanged();
    void iconPathChanged();
private:
    Q_DISABLE_COPY(SongFormat)
    QString m_description;
};

Q_DECLARE_METATYPE(SongFormat*);

QDebug operator << (QDebug dbg, SongFormat const &format);

#endif
