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
    Q_PROPERTY(QString color READ color WRITE setColor NOTIFY colorChanged FINAL)
    Q_PROPERTY(bool solidColor READ solidColor WRITE setSolidColor NOTIFY solidColorChanged FINAL)
    Q_PROPERTY(QString name READ name WRITE setName NOTIFY nameChanged FINAL)
    Q_PROPERTY(QString path READ path WRITE setPath NOTIFY pathChanged FINAL)
    Q_PROPERTY(int stretchMode READ stretchMode WRITE setStretchMode NOTIFY stretchModeChanged FINAL)
    Q_PROPERTY(bool repeatable READ repeatable WRITE setRepeatable NOTIFY repeatableChanged FINAL)
    Q_PROPERTY(bool animatable READ animatable WRITE setAnimatable NOTIFY animatableChanged FINAL)
public:
    Wallpaper(QObject * parent = 0);
    Wallpaper(QSettings const &settings, QObject * parent = 0);

    QString const& color() const;
    QString const& name() const;
    QString const& path() const;
    bool solidColor() const;
    bool repeatable() const;
    bool animatable() const;
    int  stretchMode() const;

    void setColor(QString const& color);
    void setName(QString const& name);
    void setPath(QString const& path);
    void setSolidColor(bool solidColor);
    void setRepeatable(bool repeatable);
    void setAnimatable(bool animatable);
    void setStretchMode(int mode);

    void save(QSettings & settings);

    using InstanceCounter<Wallpaper>::getInstanceCount;
    using InstanceCounter<Wallpaper>::getMaxInstanceCount;
Q_SIGNALS:
    void colorChanged();
    void solidColorChanged();
    void nameChanged();
    void pathChanged();
    void repeatableChanged();
    void animatableChanged();
    void stretchModeChanged();
private:
    QString m_color;
    QString m_name;
    QString m_path;
    bool    m_solidColor;
    bool    m_repeatable;
    bool    m_animatable;
    int     m_stretchMode;
};

Q_DECLARE_METATYPE(Wallpaper*);

#endif
