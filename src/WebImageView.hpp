#ifndef WEBIMAGEVIEW_H_
#define WEBIMAGEVIEW_H_

#include <bb/cascades/ImageView>
#include <QNetworkAccessManager>
#include <QUrl>
#include "InstanceCounter.hpp"

class WebImageView : public bb::cascades::ImageView,
                     public InstanceCounter<WebImageView> {
    Q_OBJECT
    Q_DISABLE_COPY(WebImageView)
    Q_PROPERTY (QUrl url READ url WRITE setUrl NOTIFY urlChanged)
public:
    WebImageView(bb::cascades::Container * parent = 0);
    QUrl const& url() const;
public Q_SLOTS:
    void setUrl(const QUrl& url);
signals:
    void urlChanged();
private Q_SLOTS:
    void imageLoaded(QNetworkReply* reply);
private:
    QNetworkAccessManager * m_netManager;
    QUrl m_url;
};

Q_DECLARE_METATYPE(WebImageView*);

#endif
