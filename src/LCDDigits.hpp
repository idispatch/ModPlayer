#ifndef LCDDIGITS_HPP_
#define LCDDIGITS_HPP_

#include <QMetaType>
#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>
#include <bb/cascades/ImageView>
#include "InstanceCounter.hpp"

namespace bb {
    namespace cascades {
        class Container;
    }
}

class LCDDigits : public bb::cascades::CustomControl,
                  public InstanceCounter<LCDDigits>
{
    Q_OBJECT
public:
    LCDDigits(bb::cascades::Container *parent = 0);
    ~LCDDigits();

    Q_PROPERTY(int numDigits READ numDigits WRITE setNumDigits FINAL)
    Q_PROPERTY(int number READ number WRITE setNumber RESET resetNumber FINAL)
    Q_PROPERTY(int colorScheme READ colorScheme WRITE setColorScheme FINAL)
    Q_PROPERTY(double displayScale READ displayScale WRITE setDisplayScale FINAL)

    int numDigits() const;
    void setNumDigits(int value);

    int number() const;
    void setNumber(int value);
    void resetNumber();

    double displayScale() const;
    void setDisplayScale(double value);

    int colorScheme() const;
    void setColorScheme(int value);

    static void finalize();

    using InstanceCounter<LCDDigits>::getInstanceCount;
    using InstanceCounter<LCDDigits>::getMaxInstanceCount;
private:
    Q_DISABLE_COPY(LCDDigits)
    void createLCD();

    static bb::cascades::Image images[22];
    static bool imagesInitialized;
    static void initImages();

    bb::cascades::Container * m_rootContainer;
    bb::cascades::ImageView * m_views[8];
    double m_displayScale;
    int m_numDigits;
    int m_number;
    int m_colorScheme;
};

Q_DECLARE_METATYPE(LCDDigits*);

#endif
