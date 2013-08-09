#include "Canvas.hpp"
#include <string.h>
#include <bb/cascades/Image>
#include <bb/PixelFormat>
#include <bb/utility/ImageConverter>

using namespace bb;
using namespace bb::utility;
using namespace bb::cascades;

Canvas::Canvas(int width, int height, QObject * parent)
    : QObject(parent),
      m_img(PixelFormat::RGBA_Premultiplied, width, height) {
}

ImageData Canvas::loadImageData(const char * url) {
    return ImageConverter::decode(QUrl(url), PixelFormat::RGBA_Premultiplied);
}

int Canvas::width() const {
    return m_img.width();
}

int Canvas::height() const {
    return m_img.height();
}

Image Canvas::image() const {
    return Image(m_img);
}
void Canvas::blit(int x, int y, ImageData const& from) {
    if(x < 0 || y < 0)
        return;

    const int from_w = from.width();
    const int from_h = from.height();

    if(x + from_w > width() || y + from_h > height())
        return;

    const int src_stride = from.bytesPerLine();
    const int dst_stride = m_img.bytesPerLine();

    const unsigned char * src = from.pixels();
          unsigned char * dst_ptr = m_img.pixels();
          unsigned char * dst = dst_ptr + y * dst_stride + x * sizeof(unsigned int);

    for(int row = 0; row < from_h; ++row) {
        memcpy(dst, src, from_w * sizeof(unsigned int));
        src += src_stride;
        dst += dst_stride;
    }
}
