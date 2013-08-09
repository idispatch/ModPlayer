#include "LCD.hpp"
#include <QDebug>
#include <QElapsedTimer>
#include <bb/cascades/Image>
#include <bb/cascades/Container>
#include <bb/cascades/ImageView>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>

using namespace bb::cascades;

LCD::LCD(Container *parent)
    : CustomControl(parent),
      m_timer(new QTimer(this)),
      m_rootContainer(NULL),
      m_image_on(QUrl("asset:///images/led_on.png")),
      m_image_off(QUrl("asset:///images/led_off.png")),
      m_numLetters(15),
      m_rows(8),
      m_columns(6),
      m_scroll(0),
      m_wiggle(0),
      m_wiggleDirection(1) {
    createLCD();
    bool rc;
    rc = connect(m_timer,
                 SIGNAL(timeout()),
                 this,
                 SLOT(onUpdateTimeout()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void LCD::createLCD(){
    const size_t numDots = m_columns * m_numLetters;
    size_t row, col;
    size_t rows = m_lcdView.size(), cols = 0;
    if(rows > 0) {
        cols = m_lcdView[0].size();
    }
    for(row = 0; row < rows; ++row)
    {
        for(col = 0; col < cols; ++col)
        {
            ImageView *& view = m_lcdView[row][col];
            if(view == NULL)
                continue;
            view->setParent(NULL);
            delete view;
            view = NULL;
        }
    }

    m_lcd.resize(m_rows);
    m_lcdView.resize(m_rows);
    for(size_t row = 0; row < m_rows; ++row)
    {
        m_lcd[row].resize(numDots);
        m_lcdView[row].resize(numDots);

        std::fill(m_lcdView[row].begin(),
                  m_lcdView[row].end(),
                  (ImageView*)NULL);
    }

    if(m_rootContainer != NULL)
    {
        setRoot(NULL);
        delete m_rootContainer;
        m_rootContainer = NULL;
    }

    m_rootContainer = Container::create().top(0)
                                         .topMargin(0)
                                         .bottom(0)
                                         .bottomMargin(0)
                                         .left(0)
                                         .leftMargin(0)
                                         .right(0)
                                         .rightMargin(0)
                                         .layout(StackLayout::create()
                                                     .orientation(LayoutOrientation::LeftToRight));


    for(col = 0; col < numDots; ++col) {
        Container * column = Container::create().top(0)
                                                .topMargin(0)
                                                .bottom(0)
                                                .bottomMargin(0)
                                                .left(0)
                                                .leftMargin(0)
                                                .right(0)
                                                .rightMargin(0)
                                                .layout(StackLayout::create()
                                                          .orientation(LayoutOrientation::TopToBottom));
        for(row = 0; row < m_rows; ++row)
        {
            ImageView * cell = ImageView::create()
                                    .image(m_image_off)
                                    .preferredWidth(8)
                                    .preferredHeight(8)
                                    .topMargin(0)
                                    .bottomMargin(0)
                                    .leftMargin(0)
                                    .rightMargin(0);
            m_lcdView[row][col] = cell;
            m_lcd[row][col] = false;
            column->add(cell);
        }

        m_rootContainer->add(column);
    }

    setRoot(m_rootContainer);
}

unsigned LCD::numLetters() const {
    return m_numLetters;
}

void LCD::setNumLetters(unsigned value) {
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
        updateLCD();
    }
}

unsigned LCD::scroll() const {
    return m_scroll;
}

void LCD::setScroll(unsigned value) {
    if(m_scroll != value)
    {
        m_scroll = value;
        updateLCD();
    }
}

QString const& LCD::text() const {
    return m_text;
}

void LCD::setText(QString const& text) {
    if(text != m_text)
    {
        m_text = text;
        updateLCD();
        emit textChanged();
        const unsigned numDots = m_columns * m_numLetters;
        const unsigned requiredDots = m_columns * m_text.length();
        m_wiggle = requiredDots > numDots ? requiredDots - numDots : 0;
        if(m_wiggle > 0)
        {
            //m_timer->start(200);
        }
        else
        {
            m_timer->stop();
        }
        setScroll(0);
    }
}

void LCD::updateLCD() {
    QElapsedTimer timer;
    timer.start();

    const size_t numDots = m_columns * m_numLetters;
    int length = m_text.length();
    for(size_t row = 0; row < m_rows; ++row)
    {
        for(size_t col = 0; col < numDots; ++col)
        {
            size_t index = ((col + m_scroll) / m_columns);
            QChar letter = (index >= (unsigned)length) ? QChar(' ') : m_text[index];

            const unsigned ascii = (const unsigned)letter.toAscii();
            const unsigned bitmask = console_font_6x8[ascii * m_rows + row];
            const unsigned shift = (col + m_scroll) % m_columns;
            const bool is_set = (bitmask & (0x80 >> shift));

            if(m_lcd[row][col] != is_set)
            {
                ImageView * cell = m_lcdView[row][col];
                cell->setImage(is_set ? m_image_on : m_image_off);
                m_lcd[row][col] = is_set;
            }
        }
    }

    qDebug() << "LCD:" << timer.nsecsElapsed() << "ns" << timer.elapsed() << "ms";
}

void LCD::onUpdateTimeout() {
    unsigned currentScroll = scroll();
    unsigned newScroll = currentScroll;
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
        if(currentScroll < (unsigned)-m_wiggleDirection)
        {
            m_wiggleDirection = -m_wiggleDirection;
            return;
        }
        newScroll = currentScroll + m_wiggleDirection;
    }
    setScroll(newScroll);
}
