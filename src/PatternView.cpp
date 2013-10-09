#include <QDebug>
#include <bb/cascades/Image>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/ImageView>
#include <bb/cascades/DockLayout>
#include "PatternView.hpp"

using namespace bb::cascades;

template<>
int InstanceCounter<PatternView>::s_count;
template<>
int InstanceCounter<PatternView>::s_maxCount;

PatternView::PatternView(Container *parent)
    : CustomControl(parent),
      m_mutex(QMutex::NonRecursive),
      m_rootContainer(NULL),
      m_song(NULL),
      m_canvas(NULL)
{
    setRoot(NULL);
}

void PatternView::createPatternView()
{
    QMutexLocker locker(&m_mutex);
    if(m_rootContainer == NULL)
    {
        m_rootContainer = Container::create().background(Color::Transparent)
                                             .horizontal(HorizontalAlignment::Center)
                                             .implicitLayoutAnimations(false)
                                             .layout(DockLayout::create());
        setRoot(m_rootContainer);
    }
    m_rootContainer->removeAll();
    if(m_song != NULL && m_song->songLoaded())
    {
        ImageView * patternImage = ImageView::create()
                    .horizontal(HorizontalAlignment::Center)
                    .vertical(VerticalAlignment::Center)
                    .loadEffect(ImageViewLoadEffect::None)
                    .implicitLayoutAnimations(false)
                    .scalingMethod(ScalingMethod::Fill);

        updateCanvas();
        if(m_canvas != NULL) {
            patternImage->setImage(m_canvas->image());
        }
        m_rootContainer->add(patternImage);
    }
}

void PatternView::updateCanvas() {
    if(m_canvas != NULL) {
        m_canvas->setParent(0);
        delete m_canvas;
        m_canvas = NULL;
    }

    int channels = m_song->channels();
    if(channels < 0)
        channels = 0;
    if(channels > 4)
        channels = 4;

    int pattern = m_song->currentPattern();
    int rows = 0;
    ModPlugNote* data = m_song->getPattern(pattern, &rows);

    if(data != NULL && rows > 0 && channels > 0)
    {
        const int fontWidth = 6;
        const int fontHeight = 8;
        const int width = channels * 8 * fontWidth; // 8 chars per channel
        const int height = rows * fontHeight;
        m_canvas = new Canvas(width, height, this);
    }
}

QVariant PatternView::song() const {
    return QVariant::fromValue(m_song);
}

void PatternView::setSong(QVariant value) {
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

            rc = QObject::connect(m_song, SIGNAL(currentPatternChanged()),
                                  this,   SLOT(onCurrentPatternChanged()));
            Q_ASSERT(rc);

            rc = QObject::connect(m_song, SIGNAL(currentRowChanged()),
                                  this,   SLOT(onCurrentRowChanged()));
            Q_ASSERT(rc);
            Q_UNUSED(rc);
        }

        createPatternView();
    }
}

void PatternView::onSongLoadedChanged() {
    createPatternView();
}

void PatternView::onChannelsChanged() {
    createPatternView();
}

void PatternView::onCurrentRowChanged() {
}

void PatternView::onCurrentPatternChanged() {
    createPatternView();
}
