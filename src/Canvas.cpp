#include <string.h>
#include <bb/cascades/Image>
#include <bb/PixelFormat>
#include <bb/utility/ImageConverter>

#include "Canvas.hpp"
#include "lcd_fonts.hpp"

using namespace bb;
using namespace bb::utility;
using namespace bb::cascades;

template<>
int InstanceCounter<Canvas>::s_count;
template<>
int InstanceCounter<Canvas>::s_maxCount;

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

void Canvas::print(int x, int y, unsigned fg, unsigned bg, const char * str) {
    if(str == NULL || *str == '\0')
        return;
    const int fontWidth = 6;
    const int dst_stride = m_img.bytesPerLine();
    const int dst_stride_dw = (dst_stride >> 2) - fontWidth;
    unsigned char * dst_ptr = m_img.pixels();
    unsigned char * dst_row = dst_ptr + y * dst_stride;
    for(int n = 0; str[n] != 0; ++n) // for each letter
    {
        const char p = str[n];
        unsigned int *  dst = reinterpret_cast<unsigned int*>(dst_row) + n * fontWidth + x;
        const unsigned int offset = static_cast<const unsigned int>(p); // offset in char raster
        for(int ly = 0; ly < 8; ++ly) // for each char scan line
        {
            const unsigned char scan = lcd_font_6x8[(offset << 3) + ly];
            for(int lx = 0; lx < fontWidth; ++lx) // for each char scan line dot
            {
                *dst++ = (scan & (0x80 >> lx)) ? fg : bg;
            }
            dst += dst_stride_dw;
        }
    }
}
