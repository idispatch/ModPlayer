#ifndef LCDDISPLAY_HPP_
#define LCDDISPLAY_HPP_

#include <vector>
#include <map>
#include <bb/cascades/CustomControl>
#include <bb/cascades/Image>
#include "Canvas.hpp"

namespace bb {
    namespace cascades {
        class Container;
        class ImageView;
    }
    class ImageData;
}

class LCDDisplay : public bb::cascades::CustomControl
{
    Q_OBJECT
public:
    LCDDisplay(bb::cascades::Container *parent = 0);

    Q_PROPERTY(int numLetters READ numLetters WRITE setNumLetters)
    Q_PROPERTY(int scroll READ scroll WRITE setScroll)
    Q_PROPERTY(QString text READ text WRITE setText)

    int scroll() const;
    void setScroll(int value);

    int numLetters() const;
    void setNumLetters(int value);

    QString const& text() const;
    void setText(QString const& text);

private slots:
    void onUpdateTimeout();
private:
    void createLCD();
    void updateLCD();

    QTimer * m_timer;
    bb::cascades::Container * m_rootContainer;
    bb::ImageData m_image_on;
    bb::ImageData m_image_off;

    QString m_text;
    int m_numLetters;
    int m_rows;
    int m_columns;
    Canvas * m_canvas;

    int m_scroll;
    int m_wiggle;
    int m_wiggleDirection;
};

#endif
