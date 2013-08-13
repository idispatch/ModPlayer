#ifndef SONGFORMAT_HPP_
#define SONGFORMAT_HPP_

#include "ItemGroupBase.hpp"

class SongFormat : public ItemGroupBase {
    Q_OBJECT
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged FINAL)
public:
    SongFormat(int id,
               QString const& name,
               QString const& description,
               int count,
               QObject *parent = 0);
    ~SongFormat();
    QString const& description() const;
Q_SIGNALS:
    void descriptionChanged();
private:
    Q_DISABLE_COPY(SongFormat)
    QString m_description;
};

Q_DECLARE_METATYPE(SongFormat*);

QDebug operator << (QDebug dbg, SongFormat const &format);

#endif
