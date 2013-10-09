#include <QDebug>
#include <bb/cascades/Image>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/ImageView>
#include <bb/cascades/DockLayout>
#include <bb/cascades/StackLayout>
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
                                             .horizontal(HorizontalAlignment::Fill)
                                             .implicitLayoutAnimations(false)
                                             .layout(StackLayout::create());
        setRoot(m_rootContainer);
    }

    m_rootContainer->removeAll();

    if(m_song != NULL && m_song->songLoaded())
    {
        Container * c = Container::create().background(Color::Transparent)
                                           .horizontal(HorizontalAlignment::Fill)
                                           .implicitLayoutAnimations(false)
                                           .layout(DockLayout::create());
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
        c->add(patternImage);

        m_rootContainer->add(c);
    }
}

void PatternView::updateCanvas() {
    if(m_canvas != NULL) {
        m_canvas->setParent(0);
        delete m_canvas;
        m_canvas = NULL;
    }

    int channels = m_song->channels();
    if(channels < 0) {
        channels = 0;
    }
    if(channels > 4) {
        channels = 4;
    }

    const int pattern = m_song->currentPattern();
    int rows = 0;
    ModPlugNote* data = m_song->getPattern(pattern, &rows);

    if(data != NULL && rows > 0 && channels > 0)
    {
        const int scale = 2;
        const int fontWidth = 6;
        const int fontHeight = 8;
        const int scaledFontWidth = fontWidth << (scale - 1);
        const int scaledFontHeight = fontHeight << (scale - 1);
        const int charsPerChannel = 10;

        const int indent = 5;
        const int width = indent * scaledFontWidth +
                          channels * scaledFontWidth * charsPerChannel; // 16 chars per channel
        const int height = rows * scaledFontHeight;

#define MAKE_RGBA(R,G,B,A) ((A << 24) + (B << 16) + (G << 8) + (R << 0))
        static const unsigned int rownumber_foreground_color = MAKE_RGBA(0x80, 0x80, 0x80, 0xff);
        static const unsigned int note_ative_foreground_color = MAKE_RGBA(0xff, 0xff, 0xff, 0xff);
        static const unsigned int note_idle_foreground_color = MAKE_RGBA(0x70, 0x70, 0x70, 0xff);
        static const unsigned int background_color = MAKE_RGBA(0x00, 0x00, 0x00, 0xff);
        static const unsigned int fill_color = MAKE_RGBA(0x00, 0x00, 0x00, 0xff);
#undef MAKE_RGBA

        static const unsigned char NOTE_NONE = 0;
        static const unsigned char NOTE_MIN = 1;
        static const unsigned char NOTE_MAX = 120;
        static const unsigned char NOTE_KEYOFF = 0xff;
        static const unsigned char NOTE_NOTECUT = 0xfe;
        static const unsigned char NOTE_FADE = 0xfd;
        static const unsigned char NOTE_PC = 0xfc;
        static const unsigned char NOTE_PCS = 0xfb;

        static const char * NoteNames[NOTE_MAX] = {
            "C-0", "C#0", "D-0", "D#0", "E-0", "F-0", "F#0", "G-0", "G#0", "A-0", "A#0", "B-0", // [00..11]
            "C-1", "C#1", "D-1", "D#1", "E-1", "F-1", "F#1", "G-1", "G#1", "A-1", "A#1", "B-1", // [12..23]
            "C-2", "C#2", "D-2", "D#2", "E-2", "F-2", "F#2", "G-2", "G#2", "A-2", "A#2", "B-2", // [24..35]
            "C-3", "C#3", "D-3", "D#3", "E-3", "F-3", "F#3", "G-3", "G#3", "A-3", "A#3", "B-3", // [36..47]
            "C-4", "C#4", "D-4", "D#4", "E-4", "F-4", "F#4", "G-4", "G#4", "A-4", "A#4", "B-4", // [48..59]
            "C-5", "C#5", "D-5", "D#5", "E-5", "F-5", "F#5", "G-5", "G#5", "A-5", "A#5", "B-5", // [60..71]
            "C-6", "C#6", "D-6", "D#6", "E-6", "F-6", "F#6", "G-6", "G#6", "A-6", "A#6", "B-6", // [72..83]
            "C-7", "C#7", "D-7", "D#7", "E-7", "F-7", "F#7", "G-7", "G#7", "A-7", "A#7", "B-7", // [84..95]
            "C-8", "C#8", "D-8", "D#8", "E-8", "F-8", "F#8", "G-8", "G#8", "A-8", "A#8", "B-8", // [96..107]
            "C-9", "C#9", "D-9", "D#9", "E-9", "F-9", "F#9", "G-9", "G#9", "A-9", "A#9", "B-9", // [108..119]
        };

        m_canvas = new Canvas(width, height, this);
        m_canvas->fill(fill_color);

        const ModPlugNote * note = data;

        for(int row = 0; row < rows; ++row)
        {
            char buffer[24];
            snprintf(buffer, sizeof(buffer), "%3d.", row);
            m_canvas->print(0,
                            row * scaledFontHeight,
                            rownumber_foreground_color,
                            background_color,
                            scale,
                            buffer);

            for(int channel = 0; channel < channels; ++channel)
            {
                unsigned int color;
                strcpy(buffer, "... .. ..");
                if(note->Instrument != 0)
                {
                    buffer[4] = (note->Instrument / 10) + '0';
                    buffer[5] = (note->Instrument % 10) + '0';
                    color = note_ative_foreground_color;
                }
                else
                {
                    color = note_idle_foreground_color;
                }

                switch(note->Note)
                {
                case NOTE_NONE:
                    break;
                case NOTE_KEYOFF:
                    buffer[0] = buffer[1] = buffer[2] = '=';
                    color = note_ative_foreground_color;
                    break;
                case NOTE_NOTECUT:
                    buffer[0] = buffer[1] = buffer[2] = '^';
                    color = note_ative_foreground_color;
                    break;
                case NOTE_FADE:
                    buffer[0] = buffer[1] = buffer[2] = '~';
                    color = note_ative_foreground_color;
                    break;
                case NOTE_PC:
                    buffer[0] = 'P';
                    buffer[1] = 'C';
                    buffer[2] = ' ';
                    color = note_ative_foreground_color;
                    break;
                case NOTE_PCS:
                    buffer[0] = 'P';
                    buffer[1] = 'C';
                    buffer[2] = 'S';
                    color = note_ative_foreground_color;
                    break;
                default:
                    if(note->Note >= NOTE_MIN && note->Note <= NOTE_MAX)
                    {
                        const char * noteString = NoteNames[note->Note - NOTE_MIN];
                        buffer[0] = noteString[0];
                        buffer[1] = noteString[1];
                        buffer[2] = noteString[2];
                        color = note_ative_foreground_color;
                    }
                    break;
                }

                note++;

                m_canvas->print(channel * charsPerChannel * scaledFontWidth + indent * scaledFontWidth,
                                row * scaledFontHeight,
                                color,
                                background_color,
                                scale,
                                buffer);
            }
        }
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
