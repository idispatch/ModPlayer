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

    int width() const;
    int height() const;
    bb::cascades::Image image() const;
    void blit(int x, int y, bb::ImageData const& from);
    void print(int x, int y, unsigned fg, unsigned bg, const char * str);

    static bb::ImageData loadImageData(const char * url);

    using InstanceCounter<Canvas>::getInstanceCount;
    using InstanceCounter<Canvas>::getMaxInstanceCount;
private:
    Q_DISABLE_COPY(Canvas)
    bb::ImageData m_img;
};

#endif
