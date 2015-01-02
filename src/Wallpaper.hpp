#ifndef WALLPAPER_HPP_
#define WALLPAPER_HPP_

#include <QObject>
#include <QMetaType>
#include <QSettings>
#include "InstanceCounter.hpp"

class Wallpaper : public QObject,
                  public InstanceCounter<Wallpaper> {
    Q_OBJECT
    Q_DISABLE_COPY(Wallpaper)
private:
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
    Q_PROPERTY(bool repeatable READ repeatable WRITE setRepeatable NOTIFY repeatableChanged FINAL)
    Q_PROPERTY(bool animatable READ animatable WRITE setAnimatable NOTIFY animatableChanged FINAL)
public:
    Wallpaper(QObject * parent = 0);
    Wallpaper(QSettings const &settings, QObject * parent = 0);

    Wallpaper(QString const& name,
              QString const& path,
              bool repeatable,
              bool animatable,
              QObject * parent = 0);

    QString const& name() const;
    QString const& path() const;
    bool repeatable() const;
    bool animatable() const;

    void setName(QString const& name);
    void setPath(QString const& path);
    void setRepeatable(bool repeatable);
    void setAnimatable(bool animatable);

    using InstanceCounter<Wallpaper>::getInstanceCount;
    using InstanceCounter<Wallpaper>::getMaxInstanceCount;
Q_SIGNALS:
    void nameChanged();
    void pathChanged();
    void repeatableChanged();
    void animatableChanged();
private:
    QString m_name;
    QString m_path;
    bool    m_repeatable;
    bool    m_animatable;
};

Q_DECLARE_METATYPE(Wallpaper*);

#endif
