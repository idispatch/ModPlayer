#ifndef ITEMGROUPBASE_HPP_
#define ITEMGROUPBASE_HPP_

#include <QDebug>
#include <QMetaType>
#include <QObject>
#include "InstanceCounter.hpp"

class ItemGroupBase : public QObject,
                      public InstanceCounter<ItemGroupBase> {
    Q_OBJECT
    Q_PROPERTY(int id READ id NOTIFY idChanged FINAL)
    Q_PROPERTY(QString name READ name NOTIFY nameChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(double duration READ duration NOTIFY durationChanged FINAL)
public:
    ItemGroupBase(int id,
                  QString const& name,
                  int count,
                  double duration,
                  QObject *parent);
    ~ItemGroupBase();
    int id() const;
    QString const& name() const;
    int count() const;
    double duration() const;

    using InstanceCounter<ItemGroupBase>::getInstanceCount;
    using InstanceCounter<ItemGroupBase>::getMaxInstanceCount;
Q_SIGNALS:
    void idChanged();
    void nameChanged();
    void countChanged();
    void durationChanged();
private:
    Q_DISABLE_COPY(ItemGroupBase)

    int m_id;
    QString m_name;
    int m_count;
    double m_duration;
};

Q_DECLARE_METATYPE(ItemGroupBase*);

QDebug operator << (QDebug dbg, ItemGroupBase const &group);

#endif
