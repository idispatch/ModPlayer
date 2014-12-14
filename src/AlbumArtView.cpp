#include "AlbumArtView.hpp"
#include <bb/cascades/Image>
#include "InstanceCounter.hpp"

template<>
int InstanceCounter<AlbumArtView>::s_count;
template<>
int InstanceCounter<AlbumArtView>::s_maxCount;

AlbumArtView::AlbumArtView(bb::cascades::Container *parent)
    : bb::cascades::ImageView(parent) {
}

QString const& AlbumArtView::fileName() const {
    return m_fileName;
}

void AlbumArtView::setFileName(QString const& fileName) {
    if(fileName != m_fileName) {
        m_fileName = fileName;
        emit fileNameChanged();

        QFile file("/accounts/1000/shared/photos/IMG_00000063.jpg");
        file.open(QIODevice::ReadOnly);
        QByteArray data = file.readAll();
        setImage(bb::cascades::Image(data));
    }
}
