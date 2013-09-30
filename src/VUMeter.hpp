#ifndef VUMETER_HPP_
#define VUMETER_HPP_

#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>
#include "SongModule.hpp"
#include "InstanceCounter.hpp"
#include <vector>

namespace bb {
    namespace cascades {
        class Container;
        class ImageView;
    }
}

class VUMeter : public bb::cascades::CustomControl,
                public InstanceCounter<VUMeter>
{
    Q_OBJECT
public:
    VUMeter(bb::cascades::Container *parent = 0);

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
    void onChannelVUChanged(int channel, int channelVU);
private:
    void createVU();
private:
    QMutex m_mutex;
    bb::cascades::Container * m_rootContainer;
    SongModule * m_song;
    bb::cascades::Image m_image_vu_on;
    bb::cascades::Image m_image_vu_off;
    std::vector<bb::cascades::ImageView*> m_bars;
};

Q_DECLARE_METATYPE(VUMeter*);

#endif
