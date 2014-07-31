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

Canvas::~Canvas() {
#ifdef VERBOSE_LOGGING
    qDebug() << "Canvas::~Canvas()";
#endif
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

void Canvas::hline(int x0, int x1, int y, unsigned color) {
    const int dst_stride = m_img.bytesPerLine();
    const int width = m_img.width();
    const int height = m_img.height();
    if(y < 0 || y >= height)
        return;
    if(x0 > x1) {
        int t = x0;
        x0 = x1;
        x1 = t;
    }
    if(x0 < 0)
        return;
    if(x1 > width || x1 == x0)
        return;
    unsigned char * dst = m_img.pixels() + y * dst_stride + (x0 << 2);
    unsigned * dst_ptr = reinterpret_cast<unsigned*>(dst);
    for(int col = x0; col < x1; ++col)
    {
        *dst_ptr++ = color;
    }
}

void Canvas::vline(int x, int y0, int y1, unsigned color) {
    const int dst_stride = m_img.bytesPerLine();
    const int dst_stride_dw = dst_stride >> 2;
    const int width = m_img.width();
    const int height = m_img.height();
    if(x < 0 || x >= width)
        return;
    if(y0 > y1) {
        int t = y0;
        y0 = y1;
        y1 = t;
    }
    if(y0 < 0)
        return;
    if(y1 > height || y1 == y0)
        return;
    unsigned char * dst = m_img.pixels() + y0 * dst_stride + (x << 2);
    unsigned * dst_ptr = reinterpret_cast<unsigned*>(dst);
    for(int row = y0; row < y1; ++row)
    {
        *dst_ptr = color;
        dst_ptr += dst_stride_dw;
    }
}

void Canvas::fill(unsigned color) {
    const int dst_stride = m_img.bytesPerLine();
    const int width = m_img.width();
    const int height = m_img.height();
    for(int row = 0; row < height; ++row)
    {
        unsigned char * dst = m_img.pixels();
        dst += row * dst_stride;
        unsigned * dst_ptr = reinterpret_cast<unsigned*>(dst);
        for(int col = 0; col < width; ++col)
        {
            *dst_ptr++ = color;
        }
    }
}

void Canvas::print(int x, int y, unsigned fg, unsigned bg, int scale, const char * str) {
    if(str == NULL || *str == '\0' || scale <= 0)
        return;
    const int fontWidth = 6;
    const int scaledFontWidth = fontWidth * scale;
    const int fontHeight = 8;
    const int dst_stride = m_img.bytesPerLine();
    const int dst_stride_dw = (dst_stride >> 2) - fontWidth * scale;
    unsigned char * dst_ptr = m_img.pixels();
    unsigned char * dst_row = dst_ptr + y * dst_stride;
    for(int i = 0; str[i] != 0; ++i) // for each letter
    {
        const char p = str[i];
        unsigned int *  dst = reinterpret_cast<unsigned int*>(dst_row) + i * scaledFontWidth + x;
        const unsigned int offset = static_cast<const unsigned int>(p); // offset in char raster
        for(int ly = 0; ly < fontHeight; ++ly) // for each char scan line
        {
            const unsigned char scan = lcd_font_6x8[(offset << 3) + ly];
            for(int m = 0; m < scale; ++m)
            {
                for(int lx = 0; lx < fontWidth; ++lx) // for each char scan line dot
                {
                    const unsigned px_color = (scan & (0x80 >> lx)) ? fg : bg;
                    switch(scale)
                    {
                    default:
                        /* no break */
                    case 4:
                        *dst++ = px_color;
                        /* no break */
                    case 3:
                        *dst++ = px_color;
                        /* no break */
                    case 2:
                        *dst++ = px_color;
                        /* no break */
                    case 1:
                        *dst++ = px_color;
                        break;
                    case 0:
                        break;
                    }
                }
                dst += dst_stride_dw;
            }
        }
    }
}
