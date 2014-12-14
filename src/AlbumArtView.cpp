#include "AlbumArtView.hpp"
#include <bb/cascades/Image>
#include "InstanceCounter.hpp"

#include "libid3tag/id3_id3tag.h"
#include "libid3tag/id3_utf8.h"

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
        loadAlbumArt();
        emit fileNameChanged();
    }
}

void AlbumArtView::loadAlbumArt() {
    QByteArray data;

    if(m_fileName.isEmpty())
    {
        setImage(bb::cascades::Image(data));
        setVisible(false);
        return;
    }

    ::id3_file * mp3file = ::id3_file_open(m_fileName.toUtf8().constData(),
                                           ID3_FILE_MODE_READONLY);
    do
    {
        if(mp3file == NULL) {
            break;
        }

        ::id3_tag * tag = ::id3_file_tag(mp3file);
        if(tag == NULL) {
            break;
        }

        int pic_index = 0;
        struct ::id3_frame* frame;

        frame = ::id3_tag_findframe (tag, "APIC", pic_index);
        if(frame == NULL) {
            break;
        }

        union ::id3_field* field;
        unsigned char const * tagBytes;
        ::id3_length_t length;

        field = ::id3_frame_field(frame, 1);
        tagBytes = ::id3_field_getlatin1(field);
        if(!tagBytes) {
            break;
        }

        if(strncmp(reinterpret_cast<const char*>(tagBytes), "image/", 6)) {
            break;
        }

        field = ::id3_frame_field(frame, 4);
        tagBytes = ::id3_field_getbinarydata(field, &length);
        if (!tagBytes) {
            break;
        }

        if(length > 0) {
            data.append(reinterpret_cast<const char*>(tagBytes), length);
        }

    } while(false);

    if(mp3file != NULL) {
        ::id3_file_close(mp3file);
    }

    setImage(bb::cascades::Image(data));
    setVisible(data.size() > 0);
}
