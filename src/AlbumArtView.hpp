#ifndef ALBUMARTVIEW_H_
#define ALBUMARTVIEW_H_

#include <bb/cascades/ImageView>
#include "InstanceCounter.hpp"

class AlbumArtView : public bb::cascades::ImageView,
                     public InstanceCounter<AlbumArtView> {
    Q_OBJECT
    Q_DISABLE_COPY(AlbumArtView)
    Q_PROPERTY (QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
public:
    AlbumArtView(bb::cascades::Container * parent = 0);
    QString const& fileName() const;
public Q_SLOTS:
    void setFileName(const QString &fileName);
signals:
    void fileNameChanged();
private:
    QString m_fileName;
};

Q_DECLARE_METATYPE(AlbumArtView*);

#endif
