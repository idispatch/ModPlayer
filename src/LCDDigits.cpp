#include "LCDDigits.hpp"
#include <bb/cascades/Container>
#include <bb/cascades/Color>
#include <bb/cascades/StackLayout>

using namespace bb::cascades;

bool LCDDigits::imagesInitialized = false;
Image LCDDigits::images[11];

LCDDigits::LCDDigits(Container *parent)
    : CustomControl(parent),
      m_rootContainer(NULL),
      m_numDigits(3),
      m_number(-1) {
    initImages();
    createLCD();
}

LCDDigits::~LCDDigits() {
    //qDebug() << "LCDDigits::~LCDDigits()";
}

void LCDDigits::initImages() {
    if(!imagesInitialized)
    {
        for(unsigned i = 0; i < 11; i++)
        {
            images[i] = Image(QUrl(QString("asset:///images/lcd/lcd-%1.png").arg(i)));
        }
        imagesInitialized = true;
    }
}

void LCDDigits::finalize() {
    if(imagesInitialized)
    {
        for(unsigned i = 0; i < 11; i++)
        {
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
                newImageNumber = 10;
            } else {
                if(value >= m || (value == 0 && m == 1)) {
                    newImageNumber = (value / m) % 10;
                } else {
                    newImageNumber = 10;
                }
            }

            if(m_number < 0) {
                oldImageNumber = 10;
            } else {
                if(m_number >= m || (m_number == 0 && m == 1)) {
                    oldImageNumber = (m_number / m) % 10;
                } else {
                    oldImageNumber = 10;
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
                                         .layout(StackLayout::create()
                                                    .orientation(LayoutOrientation::LeftToRight));

    int m = 1;
    for(int i = 1; i < m_numDigits; ++i) {
        m *= 10;
    }

    for(int i = m_numDigits - 1; i >= 0; --i) {
        int imageNumber;
        if(m_number < 0) {
            imageNumber = 10;
        } else {
            if(m_number >= m || (m_number == 0 && m == 1)) {
                imageNumber = (m_number / m) % 10;
            } else {
                imageNumber = 10;
            }
            m /= 10;
        }

        ImageView * view = ImageView::create()
                                      .topMargin(0)
                                      .bottomMargin(0)
                                      .leftMargin(0)
                                      .rightMargin(0)
                                      .preferredWidth(32)
                                      .preferredHeight(44)
                                      .loadEffect(ImageViewLoadEffect::None)
                                      .scalingMethod(ScalingMethod::Fill)
                                      .implicitLayoutAnimations(false)
                                      .image(images[imageNumber]);
        m_views[i] = view;
        m_rootContainer->add(view);
    }

    setRoot(m_rootContainer);
}
