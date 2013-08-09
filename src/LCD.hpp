#ifndef LCD_HPP_
#define LCD_HPP_

#include <vector>
#include <map>
#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>

namespace bb {
    namespace cascades {
        class Container;
        class ImageView;
    }
}

class LCD : public bb::cascades::CustomControl
{
    Q_OBJECT
public:
    LCD(bb::cascades::Container *parent = 0);

    Q_PROPERTY(unsigned numLetters READ numLetters WRITE setNumLetters)
    Q_PROPERTY(unsigned scroll READ scroll WRITE setScroll)
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

    unsigned scroll() const;
    void setScroll(unsigned value);

    unsigned numLetters() const;
    void setNumLetters(unsigned value);

    QString const& text() const;
    void setText(QString const& text);

Q_SIGNALS:
    void textChanged();
private slots:
    void onUpdateTimeout();
private:
    void createLCD();
    void updateLCD();

    QTimer * m_timer;
    bb::cascades::Container * m_rootContainer;
    bb::cascades::Image m_image_on;
    bb::cascades::Image m_image_off;

    QString m_text;
    unsigned m_numLetters;
    unsigned m_rows;
    unsigned m_columns;
    unsigned m_scroll;
    unsigned m_wiggle;
    int m_wiggleDirection;

    std::vector<std::vector<bool> > m_lcd;
    std::vector<std::vector<bb::cascades::ImageView*> > m_lcdView;

    static unsigned char console_font_8x8[8*256];
    static unsigned char console_font_6x8[8*256];
};

#endif /* LCD_HPP_ */
