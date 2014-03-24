#include "WebImageView.hpp"
#include <bb/cascades/Image>
#include "InstanceCounter.hpp"

template<>
int InstanceCounter<WebImageView>::s_count;
template<>
int InstanceCounter<WebImageView>::s_maxCount;

WebImageView::WebImageView(bb::cascades::Container *parent)
    : bb::cascades::ImageView(parent),
      m_netManager(new QNetworkAccessManager(this)) {
    bool rc = QObject::connect(m_netManager, SIGNAL(finished(QNetworkReply*)),
                               this,  SLOT(imageLoaded(QNetworkReply*)));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

QUrl const& WebImageView::url() const {
    return m_url;
}

void WebImageView::setUrl(const QUrl& url) {
    if(m_url != url) {
        m_url = url;
        if(!m_url.isEmpty() && m_url.isValid()) {
            m_netManager->get(QNetworkRequest(m_url));
        }
        emit urlChanged();
    }
}

void WebImageView::imageLoaded(QNetworkReply* reply) {
    setImage(bb::cascades::Image(reply->readAll()));
    reply->deleteLater();
}
