#include <QDebug>
#include <bb/cascades/Image>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/ImageView>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/DockLayout>
#include "VUMeter.hpp"

using namespace bb::cascades;

VUMeter::VUMeter(Container *parent)
    : CustomControl(parent),
      m_mutex(QMutex::NonRecursive),
      m_rootContainer(NULL),
      m_song(NULL),
      m_image_vu_on(Image(QUrl("asset:///images/vu/vu-on.png"))),
      m_image_vu_off(Image(QUrl("asset:///images/vu/vu-off.png")))
{
    setRoot(NULL);
}

void VUMeter::createVU()
{
    QMutexLocker locker(&m_mutex);
    if(m_rootContainer == NULL)
    {
        m_rootContainer = Container::create().background(Color::Transparent)
                                             .horizontal(HorizontalAlignment::Fill)
                                             .implicitLayoutAnimations(false)
                                             .layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));
        setRoot(m_rootContainer);
    }

    if(m_song == NULL || !m_song->songLoaded())
    {
        m_rootContainer->removeAll();
        m_bars.clear();
    }
    else
    {
        const int channels = m_song->channels();
        if(static_cast<int>(m_bars.size()) != channels)
        {
            m_rootContainer->removeAll();
            m_bars.resize(channels);

            m_rootContainer->add(Container::create()
                                .layoutProperties(StackLayoutProperties::create().spaceQuota(1.0))
                                .implicitLayoutAnimations(false));

            for(int i = 0; i < channels; ++i)
            {
                const float height = 100.;
                const float width = 32.;
                const float margin = 10.;

                Container * c = Container::create()
                    .layoutProperties(StackLayoutProperties::create().spaceQuota(-1.0))
                    .vertical(VerticalAlignment::Fill)
                    .leftMargin(margin)
                    .rightMargin(margin)
                    .implicitLayoutAnimations(false)
                    .layout(DockLayout::create());
                ImageView * v1 = ImageView::create()
                    .horizontal(HorizontalAlignment::Fill)
                    .vertical(VerticalAlignment::Fill)
                    .image(m_image_vu_on)
                    .loadEffect(ImageViewLoadEffect::None)
                    .implicitLayoutAnimations(false)
                    .scalingMethod(ScalingMethod::Fill)
                    .preferredSize(width, height);
                ImageView * v2 = ImageView::create()
                    .horizontal(HorizontalAlignment::Fill)
                    .vertical(VerticalAlignment::Fill)
                    .image(m_image_vu_off)
                    .loadEffect(ImageViewLoadEffect::None)
                    .implicitLayoutAnimations(false)
                    .scalingMethod(ScalingMethod::Fill)
                    .preferredSize(width, height);

                m_bars[i] = v2;

                c->add(v1);
                c->add(v2);

                m_rootContainer->add(c);
            }

            m_rootContainer->add(Container::create()
                                .layoutProperties(StackLayoutProperties::create().spaceQuota(1.0))
                                .implicitLayoutAnimations(false));
        }
    }
}

QVariant VUMeter::song() const {
    return QVariant::fromValue(m_song);
}

void VUMeter::setSong(QVariant value) {
    SongModule * song = qobject_cast<SongModule*>(value.value<QObject*>());
    if(song != m_song) {
        m_song = song;

        emit songChanged();

        bool rc;
        if(m_song != NULL)
        {
            rc = QObject::connect(m_song, SIGNAL(songLoadedChanged()),
                                  this,   SLOT(onSongLoadedChanged()));
            Q_ASSERT(rc);

            rc = QObject::connect(m_song, SIGNAL(channelsChanged()),
                                  this,   SLOT(onChannelsChanged()));
            Q_ASSERT(rc);

            rc = QObject::connect(m_song, SIGNAL(channelVUChanged(int, int)),
                                  this,   SLOT(onChannelVUChanged(int, int)));
            Q_ASSERT(rc);
            Q_UNUSED(rc);
        }

        createVU();
    }
}

void VUMeter::onSongLoadedChanged() {
    createVU();
}

void VUMeter::onChannelsChanged() {
    createVU();
}

void VUMeter::onChannelVUChanged(int channel, int channelVU) {
    QMutexLocker locker(&m_mutex);
    const int numBars = static_cast<int>(m_bars.size());
    if(channel < 0 || channel >= numBars)
        return;
    if(channelVU < 0)
        channelVU = 0;
    if(channelVU > 255)
        channelVU = 255;
    float offset = -channelVU * 100/256;
    m_bars[channel]->setTranslationY(offset);
}