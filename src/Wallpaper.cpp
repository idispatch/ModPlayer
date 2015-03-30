#include "Wallpaper.hpp"

template<>
int InstanceCounter<Wallpaper>::s_count;
template<>
int InstanceCounter<Wallpaper>::s_maxCount;

Wallpaper::Wallpaper(QObject * parent)
    : QObject(parent),
      m_name("ModPlayer Classic"),
      m_path("asset:///images/backgrounds/view_back.amd"),
      m_solidColor(false),
      m_repeatable(true),
      m_animatable(true),
      m_stretchMode(2) // Fill
{}

Wallpaper::Wallpaper(QSettings const& settings, QObject * parent)
    : QObject(parent),
      m_color(settings.value("wallpaper/color", "#ff483d8b").toString()),
      m_name(settings.value("wallpaper/name", "ModPlayer Classic").toString()),
      m_path(settings.value("wallpaper/path", "asset:///images/backgrounds/view_back.amd").toString()),
      m_solidColor(settings.value("wallpaper/solidColor", false).toBool()),
      m_repeatable(settings.value("wallpaper/repeatable", false).toBool()),
      m_animatable(settings.value("wallpaper/animatable", true).toBool()),
      m_stretchMode(settings.value("wallpaper/stretchMode", 2).toInt()) {
}

void Wallpaper::save(QSettings & settings) {
    settings.beginGroup("wallpaper");
    settings.setValue("color", color());
    settings.setValue("name", name());
    settings.setValue("path", path());
    settings.setValue("solidColor", solidColor());
    settings.setValue("repeatable", repeatable());
    settings.setValue("animatable", animatable());
    settings.setValue("stretchMode", stretchMode());
    settings.endGroup();
}

QString const& Wallpaper::color() const {
    return m_color;
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

int Wallpaper::stretchMode() const {
    return m_stretchMode;
}

bool Wallpaper::solidColor() const {
    return m_solidColor;
}

void Wallpaper::setColor(QString const& color) {
    if(color != m_color) {
        m_color = color;
        emit colorChanged();
    }
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

void Wallpaper::setSolidColor(bool solidColor) {
    if(solidColor != m_solidColor) {
        m_solidColor = solidColor;
        emit solidColorChanged();
    }
}

void Wallpaper::setStretchMode(int value) {
    if(value != m_stretchMode) {
        m_stretchMode = value;
        emit stretchModeChanged();
    }
}
