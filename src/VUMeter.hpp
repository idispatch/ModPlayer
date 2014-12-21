#ifndef VUMETER_HPP_
#define VUMETER_HPP_

#include <QVector>
#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>
#include "SongModule.hpp"
#include "InstanceCounter.hpp"

namespace bb {
    namespace cascades {
        class Container;
        class ImageView;
        class AbsoluteLayoutProperties;
    }
}

class VUMeter : public bb::cascades::CustomControl,
                public InstanceCounter<VUMeter>
{
    Q_OBJECT
    Q_DISABLE_COPY(VUMeter)
public:
    VUMeter(bb::cascades::Container *parent = 0);
    ~VUMeter();

    Q_PROPERTY(QVariant song READ song WRITE setSong NOTIFY songChanged FINAL)

    QVariant song() const;
    void setSong(QVariant value);

    using InstanceCounter<VUMeter>::getInstanceCount;
    using InstanceCounter<VUMeter>::getMaxInstanceCount;
Q_SIGNALS:
    void songChanged();
private slots:
    void onSongLoadedChanged();
    void onChannelsChanged();
    void onChannelVUChanged();
    void enableAnimation();
    void disableAnimation();
private:
    void createVU();
private:
    bb::cascades::Container * m_rootContainer;
    SongModule * m_song;
    bb::cascades::Image m_image_vu_on;
    bb::cascades::Image m_image_vu_off;
    bb::cascades::AbsoluteLayoutProperties* m_bars[128];
    int m_numBars;
    bool m_enabled;
};

Q_DECLARE_METATYPE(VUMeter*);

#endif
