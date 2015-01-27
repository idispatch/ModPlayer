#include "LCDDigits.hpp"
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/StackLayout>

using namespace bb::cascades;

bool LCDDigits::imagesInitialized = false;
Image LCDDigits::images[22];

template<>
int InstanceCounter<LCDDigits>::s_count;
template<>
int InstanceCounter<LCDDigits>::s_maxCount;

LCDDigits::LCDDigits(Container *parent)
    : CustomControl(parent),
      m_rootContainer(NULL),
      m_displayScale(1.0),
      m_numDigits(3),
      m_number(-1),
      m_colorScheme(0) {
    initImages();
    createLCD();
}

LCDDigits::~LCDDigits() {
}

void LCDDigits::initImages() {
    if(!imagesInitialized) {
        for(unsigned i = 0; i < sizeof(images)/sizeof(images[0]); i++) {
            if(i < 11) {
                images[i] = Image(QUrl(QString("asset:///images/lcd-dark/lcd-%1.png").arg(i)));
            } else {
                images[i] = Image(QUrl(QString("asset:///images/lcd-light/lcd-%1.png").arg(i - 11)));
            }
        }
        imagesInitialized = true;
    }
}

void LCDDigits::finalize() {
    if(imagesInitialized) {
        for(unsigned i = 0; i < sizeof(images)/sizeof(images[0]); i++) {
            images[i] = Image();
        }
        imagesInitialized = false;
    }
}

int LCDDigits::numDigits() const {
    return m_numDigits;
}

void LCDDigits::setNumDigits(int value) {
    if(value < 1) {
        value = 1;
    }
    if(value > 8) {
        value = 8;
    }
    if(m_numDigits != value) {
        m_numDigits = value;
        createLCD();
    }
}

int LCDDigits::number() const {
    return m_number;
}

void LCDDigits::setNumber(int value) {
    if(value != m_number)
    {
        for(int i = 0, m = 1; i < m_numDigits; ++i, m*=10) {
            int oldImageNumber;
            int newImageNumber;

            if(value < 0) {
                newImageNumber = 10 + (m_colorScheme * 11);
            } else {
                if(value >= m || (value == 0 && m == 1)) {
                    newImageNumber = (value / m) % 10 + (m_colorScheme * 11);
                } else {
                    newImageNumber = 10 + (m_colorScheme * 11);
                }
            }

            if(m_number < 0) {
                oldImageNumber = 10 + (m_colorScheme * 11);
            } else {
                if(m_number >= m || (m_number == 0 && m == 1)) {
                    oldImageNumber = (m_number / m) % 10 + (m_colorScheme * 11);
                } else {
                    oldImageNumber = 10 + (m_colorScheme * 11);
                }
            }

            if(oldImageNumber != newImageNumber)
            {
                m_views[i]->setImage(images[newImageNumber]);
            }
        }
        m_number = value;
    }
}

void LCDDigits::resetNumber() {
    setNumber(-1);
}

int LCDDigits::colorScheme() const {
    return m_colorScheme;
}

void LCDDigits::setColorScheme(int value) {
    if(value != m_colorScheme) {
        switch(value) {
        case 0: case 1:
            m_colorScheme = value;
            break;
        default:
            m_colorScheme = 0;
            break;
        }
        createLCD();
    }
}

double LCDDigits::displayScale() const {
    return m_displayScale;
}

void LCDDigits::setDisplayScale(double value) {
    if(value < 0.0) {
        value = 0.0;
    }
    if(m_displayScale != value) {
        m_displayScale = value;

        for(int i = 0; i < m_numDigits; ++i) {
            m_views[i]->setPreferredWidth(32 * m_displayScale);
            m_views[i]->setPreferredHeight(44 * m_displayScale);
        }
    }
}

void LCDDigits::createLCD() {
    for(unsigned i = 0;
        i < sizeof(m_views)/sizeof(m_views[0]);
        ++i) {
        m_views[i] = NULL;
    }

    if(m_rootContainer != NULL)
    {
        setRoot(NULL);
        m_rootContainer->setParent(0);
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
                                         .background(Color::Transparent)
                                         .layout(StackLayout::create().orientation(LayoutOrientation::LeftToRight));

    int m = 1;
    for(int i = 1; i < m_numDigits; ++i) {
        m *= 10;
    }

    for(int i = m_numDigits - 1; i >= 0; --i) {
        int imageNumber;
        if(m_number < 0) {
            imageNumber = 10 + (m_colorScheme * 11);
        } else {
            if(m_number >= m || (m_number == 0 && m == 1)) {
                imageNumber = (m_number / m) % 10 + (m_colorScheme * 11);
            } else {
                imageNumber = 10 + (m_colorScheme * 11);
            }
            m /= 10;
        }

        m_views[i] = ImageView::create()
                                  .topMargin(0)
                                  .bottomMargin(0)
                                  .leftMargin(0)
                                  .rightMargin(0)
                                  .preferredWidth(32 * m_displayScale)
                                  .preferredHeight(44 * m_displayScale)
                                  .loadEffect(ImageViewLoadEffect::None)
                                  .scalingMethod(ScalingMethod::Fill)
                                  .implicitLayoutAnimations(false)
                                  .image(images[imageNumber]);
        m_rootContainer->add(m_views[i]);
    }

    setRoot(m_rootContainer);
}
