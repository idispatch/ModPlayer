#include "AlbumArtView.hpp"
#include "AlbumArtLoader.hpp"
#include <bb/cascades/Image>
#include "InstanceCounter.hpp"

template<>
int InstanceCounter<AlbumArtView>::s_count;
template<>
int InstanceCounter<AlbumArtView>::s_maxCount;

AlbumArtView::AlbumArtView(bb::cascades::Container *parent)
    : bb::cascades::ImageView(parent) {
    AlbumArtLoader *imageLoader = new AlbumArtLoader();
    imageLoader->moveToThread(&m_workerThread);

    bool rc;
    rc = QObject::connect(&m_workerThread, SIGNAL(finished()),
                          imageLoader,     SLOT(deleteLater()));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    rc = QObject::connect(this,            SIGNAL(loadAlbumArt(QString)),
                          imageLoader,     SLOT(loadAlbumArt(QString)));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    rc = QObject::connect(imageLoader,     SIGNAL(resultReady(QByteArray)),
                          this,            SLOT(onAlbumArtLoaded(QByteArray)));
    Q_ASSERT(rc);
    Q_UNUSED(rc);

    m_workerThread.start(QThread::LowPriority);
}

AlbumArtView::~AlbumArtView() {
    m_workerThread.quit();
    m_workerThread.wait();
}

QString const& AlbumArtView::fileName() const {
    return m_fileName;
}

void AlbumArtView::setFileName(QString const& fileName) {
    if(fileName != m_fileName) {
        m_fileName = fileName;
        emit fileNameChanged();
        if(!m_fileName.isEmpty()) {
            emit albumArtLoading();
            loadAlbumArt(m_fileName);
        }
    }
}

void AlbumArtView::onAlbumArtLoaded(QByteArray const& data) {
    setImage(bb::cascades::Image(data));
    setVisible(data.size() > 0);
    if(!data.isEmpty()) {
        emit albumArtLoaded();
    }
}
