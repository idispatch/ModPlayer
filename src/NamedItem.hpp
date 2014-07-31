#ifndef NAMEDITEM_HPP_
#define NAMEDITEM_HPP_

#include <QDebug>
#include <QMetaType>
#include <QObject>
#include "InstanceCounter.hpp"

class NamedItem : public QObject,
                  public InstanceCounter<NamedItem>
{
    Q_OBJECT
    Q_DISABLE_COPY(NamedItem)
    Q_PROPERTY(int id READ id NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL)
public:
    NamedItem(int id,
              QString const& name,
              QObject *parent);
    ~NamedItem();
    int id() const;
    QString const& name() const;

    using InstanceCounter<NamedItem>::getInstanceCount;
    using InstanceCounter<NamedItem>::getMaxInstanceCount;
Q_SIGNALS:
    void idChanged();
    void nameChanged();
private:
    int m_id;
    QString m_name;
};

Q_DECLARE_METATYPE(NamedItem*);

QDebug operator <<(QDebug dbg, NamedItem const &item);

#endif
