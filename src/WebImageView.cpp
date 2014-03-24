#include "WebImageView.hpp"
#include <bb/cascades/Image>
#include "InstanceCounter.hpp"

template<>
int InstanceCounter<WebImageView>::s_count;
template<>
int InstanceCounter<WebImageView>::s_maxCount;

WebImageView::WebImageView(bb::cascades::Container *parent)
    : bb::cascades::ImageView(parent) {
    m_netManager = new QNetworkAccessManager(this);
}

const QUrl& WebImageView::url() const {
    return m_Url;
}

void WebImageView::setUrl(const QUrl& url) {
    if(m_Url != url) {
        m_Url = url;
        QNetworkReply * reply = m_netManager->get(QNetworkRequest(url));
        bool rc = QObject::connect(reply, SIGNAL(finished()),
                                   this,  SLOT(imageLoaded()));
        Q_ASSERT(rc);
        Q_UNUSED(rc);
        emit urlChanged();
    }
}

void WebImageView::imageLoaded() {
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    setImage(bb::cascades::Image(reply->readAll()));
}
