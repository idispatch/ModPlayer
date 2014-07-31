#ifndef CANVAS_HPP_
#define CANVAS_HPP_

#include <QObject>
#include <bb/ImageData>
#include "InstanceCounter.hpp"

namespace bb {
    namespace cascades {
        class Image;
    }
    class ImageData;
}

class Canvas : public QObject,
               public InstanceCounter<Canvas> {
    Q_OBJECT
public:
    Canvas(int width, int height, QObject * parent);
    ~Canvas();

    int width() const;
    int height() const;
    bb::cascades::Image image() const;
    void fill(unsigned color);
    void blit(int x, int y, bb::ImageData const& from);
    void hline(int x0, int x1, int y, unsigned color);
    void vline(int x, int y0, int y1, unsigned color);
    void print(int x, int y, unsigned fg, unsigned bg, int scale, const char * str);

    static bb::ImageData loadImageData(const char * url);

    using InstanceCounter<Canvas>::getInstanceCount;
    using InstanceCounter<Canvas>::getMaxInstanceCount;
private:
    Q_DISABLE_COPY(Canvas)
    bb::ImageData m_img;
};

#endif
