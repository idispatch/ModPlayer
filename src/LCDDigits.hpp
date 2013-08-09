#ifndef LCDDIGITS_HPP_
#define LCDDIGITS_HPP_

#include <QMetaType>
#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>
#include <bb/cascades/ImageView>

namespace bb {
    namespace cascades {
        class Container;
    }
}

class LCDDigits : public bb::cascades::CustomControl
{
    Q_OBJECT
public:
    LCDDigits(bb::cascades::Container *parent = 0);
    ~LCDDigits();

    Q_PROPERTY(int numDigits READ numDigits WRITE setNumDigits FINAL)
    Q_PROPERTY(int number READ number WRITE setNumber RESET resetNumber FINAL)

    int numDigits() const;
    void setNumDigits(int value);

    int number() const;
    void setNumber(int value);
    void resetNumber();

private:
    void createLCD();

    static bb::cascades::Image images[11];
    static bool imagesInitialized;
    static void initImages();

    bb::cascades::Container * m_rootContainer;
    bb::cascades::ImageView * m_views[8];
    int m_numDigits;
    int m_number;
};

Q_DECLARE_METATYPE(LCDDigits*);

#endif