#ifndef ALBUMARTVIEW_HPP
#define ALBUMARTVIEW_HPP

#include <QObject>
#include <QMetaType>
#include <QThread>
#include <bb/cascades/ImageView>
#include "InstanceCounter.hpp"

class AlbumArtView : public bb::cascades::ImageView,
                     public InstanceCounter<AlbumArtView> {
    Q_OBJECT
    Q_DISABLE_COPY(AlbumArtView)
    Q_PROPERTY (QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
public:
    AlbumArtView(bb::cascades::Container * parent = 0);
    ~AlbumArtView();
    QString const& fileName() const;
signals:
    void fileNameChanged();
    void loadAlbumArt(QString const& fileName);
    void albumArtLoading();
    void albumArtLoaded();
public slots:
    void setFileName(QString const& fileName);
    void onAlbumArtLoaded(QByteArray const& data);
private:
    QThread m_workerThread;
    QString m_fileName;
};

Q_DECLARE_METATYPE(AlbumArtView*);

#endif
