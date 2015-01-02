#include "Wallpaper.hpp"

template<>
int InstanceCounter<Wallpaper>::s_count;
template<>
int InstanceCounter<Wallpaper>::s_maxCount;

Wallpaper::Wallpaper(QObject * parent)
    : QObject(parent),
      m_repeatable(false),
      m_animatable(false) {
}

Wallpaper::Wallpaper(QString const& name,
                     QString const& path,
                     bool repeatable,
                     bool animatable,
                     QObject * parent)
    : QObject(parent),
      m_name(name),
      m_path(path),
      m_repeatable(repeatable),
      m_animatable(animatable) {
}

QString const& Wallpaper::name() const {
    return m_name;
}

QString const& Wallpaper::path() const {
    return m_path;
}

bool Wallpaper::repeatable() const {
    return m_repeatable;
}

bool Wallpaper::animatable() const {
    return m_animatable;
}

void Wallpaper::setName(QString const& name) {
    if(name != m_name) {
        m_name = name;
        emit nameChanged();
    }
}

void Wallpaper::setPath(QString const& path) {
    if(path != m_path) {
        m_path = path;
        emit pathChanged();
    }
}

void Wallpaper::setRepeatable(bool repeatable) {
    if(repeatable != m_repeatable) {
        m_repeatable = repeatable;
        emit repeatableChanged();
    }
}

void Wallpaper::setAnimatable(bool animatable) {
    if(animatable != m_animatable) {
        m_animatable = animatable;
        emit animatableChanged();
    }
}
