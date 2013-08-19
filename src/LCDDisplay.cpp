#include "LCDDisplay.hpp"
#include "lcd_fonts.hpp"

#include <QDebug>
#include <bb/cascades/Image>
#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/Color>

using namespace bb::cascades;

template<>
int InstanceCounter<LCDDisplay>::s_count;
template<>
int InstanceCounter<LCDDisplay>::s_maxCount;

LCDDisplay::LCDDisplay(Container *parent)
    : CustomControl(parent),
      m_timer(new QTimer(this)),
      m_rootContainer(NULL),
      m_image_on(Canvas::loadImageData("asset:///images/led/led_on.png")),
      m_image_off(Canvas::loadImageData("asset:///images/led/led_off.png")),
      m_numLetters(15),
      m_rows(8),
      m_columns(6),
      m_canvas(new Canvas(m_columns * m_image_on.width() * m_numLetters,
                          m_rows * m_image_on.height(),
                          this)),
      m_scroll(0),
      m_wiggle(0),
      m_wiggleDirection(1) {
    bool rc;
    rc = QObject::connect(m_timer, SIGNAL(timeout()),
                          this,    SLOT(onUpdateTimeout()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    createLCD();
}

void LCDDisplay::createLCD(){
    if(m_rootContainer != NULL)
    {
        setRoot(NULL);
        m_rootContainer->setParent(0);
        delete m_rootContainer;
        m_rootContainer = NULL;
    }

    const int width = m_columns * m_image_on.width() * m_numLetters;
    const int height = m_rows * m_image_on.height();

    m_rootContainer = Container::create().top(0)
                                         .topMargin(0)
                                         .bottom(0)
                                         .bottomMargin(0)
                                         .left(0)
                                         .leftMargin(0)
                                         .right(0)
                                         .rightMargin(0)
                                         .preferredWidth(width)
                                         .preferredHeight(height)
                                         .background(Color::Transparent)
                                         .layout(AbsoluteLayout::create());
    m_rootContainer->setMinWidth(width);
    m_rootContainer->setMaxWidth(width);
    m_rootContainer->setMinHeight(height);
    m_rootContainer->setMaxHeight(height);

    ImageView * view = ImageView::create()
                                  .topMargin(0)
                                  .bottomMargin(0)
                                  .leftMargin(0)
                                  .rightMargin(0)
                                  .loadEffect(ImageViewLoadEffect::None)
                                  .scalingMethod(ScalingMethod::None)
                                  .implicitLayoutAnimations(false)
                                  .layoutProperties(AbsoluteLayoutProperties::create().x(0).y(0));
    m_rootContainer->add(view);

    updateLCD();
    setRoot(m_rootContainer);
}

void LCDDisplay::updateLCD() {
    const int textLength = m_text.length();
    const int length = std::max(textLength, (int)m_numLetters);
    int newPixelWidth = m_columns * m_image_on.width() * length;

    if(m_canvas != NULL) {
        if(newPixelWidth != m_canvas->width()) {
            m_canvas->setParent(0);
            delete m_canvas;
            m_canvas = NULL;
        }
    }

    if(m_canvas == NULL) {
        int newPixelHeight = m_rows * m_image_on.height();
        m_canvas = new Canvas(newPixelWidth, newPixelHeight, this);
    }

    const int numDots = m_columns * length;

    for(int row = 0; row < m_rows; ++row)
    {
        for(int col = 0; col < numDots; ++col)
        {
            const int index = col / m_columns;
            const QChar letter = index > textLength ? QChar(' ') : m_text[index];

            const unsigned ascii = (const unsigned)letter.toAscii();
            const unsigned bitmask = lcd_font_6x8[ascii * m_rows + row];
            const unsigned shift = col % m_columns;
            const bool is_set = (bitmask & (0x80 >> shift));

            m_canvas->blit(col * m_image_on.width(),
                           row * m_image_on.height(),
                           (is_set ? m_image_on : m_image_off));
        }
    }

    ImageView * view = qobject_cast<ImageView *>(m_rootContainer->at(0));
    Image image = m_canvas->image();
    view->setImage(image);
}

int LCDDisplay::numLetters() const {
    return m_numLetters;
}

void LCDDisplay::setNumLetters(int value) {
    if(value < 1) {
        value = 1;
    }
    if(value > 32) {
        value = 32;
    }
    if(m_numLetters != value)
    {
        m_numLetters = value;
        createLCD();
    }
}

int LCDDisplay::scroll() const {
    return m_scroll;
}

void LCDDisplay::setScroll(int value) {
    if(value < 0) {
        value = 0;
    }
    if(m_scroll != value)
    {
        m_scroll = value;
        Control * view = m_rootContainer->at(0);
        AbsoluteLayoutProperties * p = qobject_cast<AbsoluteLayoutProperties*>(view->layoutProperties());
        p->setPositionX(-m_scroll * m_image_on.width());
    }
}

QString const& LCDDisplay::text() const {
    return m_text;
}

void LCDDisplay::setText(QString const& text) {
    if(text != m_text)
    {
        m_text = text;

        updateLCD();

        const int numDots = m_columns * m_numLetters;
        const int requiredDots = m_columns * m_text.length();
        m_wiggle = requiredDots > numDots ? requiredDots - numDots : 0;
        if(m_wiggle > 0)
        {
            m_timer->start(100);
        }
        else
        {
            m_timer->stop();
        }

        setScroll(0);
    }
}

void LCDDisplay::onUpdateTimeout() {
    int currentScroll = scroll();
    int newScroll = currentScroll;
    if(m_wiggleDirection > 0)
    {
        newScroll = currentScroll + m_wiggleDirection;
        if(newScroll > m_wiggle)
        {
            m_wiggleDirection = -m_wiggleDirection;
            return;
        }
    }
    else
    {
        if(currentScroll < -m_wiggleDirection)
        {
            m_wiggleDirection = -m_wiggleDirection;
            return;
        }
        newScroll = currentScroll + m_wiggleDirection;
    }
    setScroll(newScroll);
}
