#include "InternetRadio.hpp"
#include <QDebug>
#include <QByteArray>
#include <QRegExp>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <bb/system/InvokeManager>

using namespace bb::system;

template<>
int InstanceCounter<InternetRadio>::s_count;
template<>
int InstanceCounter<InternetRadio>::s_maxCount;

InternetRadio::InternetRadio(QObject * parent)
    : QObject(parent),
      m_networkManager(new QNetworkAccessManager(this)),
      m_networkConfigurationManager(new QNetworkConfigurationManager(this)) {
    bool rc;
    Q_UNUSED(rc);
    rc = QObject::connect(m_networkManager, SIGNAL(finished(QNetworkReply*)),
                          this,             SLOT(onHttpFinished(QNetworkReply*)));
    Q_ASSERT(rc);

    rc = QObject::connect(m_networkManager, SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                          this,             SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
    Q_ASSERT(rc);

    rc = QObject::connect(m_networkConfigurationManager, SIGNAL(onlineStateChanged(bool)),
                          this,                          SLOT(onOnlineStateChanged(bool)));
    Q_ASSERT(rc);
}

InternetRadio::~InternetRadio() {
#ifdef VERBOSE_LOGGING
    qDebug() << "InternetRadio::~InternetRadio()";
#endif
}

void InternetRadio::openNetworkConfiguration()
{
    InvokeManager invokeManager;
    InvokeRequest request;
    request.setTarget("sys.settings.target");
    request.setAction("bb.action.OPEN");
    request.setMimeType("settings/view");
    request.setUri(QUrl("settings://networkconnections"));
    invokeManager.invoke(request);
}

bool InternetRadio::isNetworkAvailable() const {
    QList<QNetworkConfiguration> active = QNetworkConfigurationManager().allConfigurations(QNetworkConfiguration::Active);
    return active.count();
}

void InternetRadio::cancelAllPendingDownloads() {
    if(m_pendingDownloads.size() > 0)
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "Cancelling" << m_pendingDownloads.size() << "downloads";
#endif
        QSet<QUrl>::const_iterator i;
        for(i = m_pendingDownloads.begin();
            i != m_pendingDownloads.end();
            ++i)
        {
            emit downloadFailure(*i);
        }
        m_pendingDownloads.clear();
        emit pendingDownloadCountChanged();
    }
}

void InternetRadio::onOnlineStateChanged(bool isOnline) {
    if(!isOnline)
    {
        cancelAllPendingDownloads();
    }
}

void InternetRadio::download(QUrl const& url) {
    if(!isNetworkAvailable())
    {
        openNetworkConfiguration();
        emit downloadFailure(url);
    }
    else
    {
        if(url.host().indexOf("diforfree.org") > 0) {
            QStringList result;
            result << url.toString();
            emit downloadFinished(url, result);
            return;
        }

        if(m_pendingDownloads.contains(url)) {
#ifdef VERBOSE_LOGGING
            qDebug() << "Already downloading" << url;
#endif
        }
        else
        {
            m_pendingDownloads.insert(url);
#ifdef VERBOSE_LOGGING
            qDebug() << "Starting downloading" << url;
#endif
            QNetworkRequest request = QNetworkRequest(url);
            m_networkManager->get(request);

            emit downloadStarted(url);
            emit pendingDownloadCountChanged();
        }
    }
}

void InternetRadio::onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility a) {
#ifdef VERBOSE_LOGGING
    qDebug() << "Network accessible changed to" << a;
#endif
    if(a != QNetworkAccessManager::Accessible)
    {
        cancelAllPendingDownloads();
    }
}

void InternetRadio::handleRedirect(QNetworkReply * reply) {
    QVariant redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    QUrl redirectURL = redirect.toUrl();
    QUrl originalURL = reply->request().url();

    if(!redirectURL.isEmpty() && redirectURL != originalURL)
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "Redirected from" << originalURL << "to" << redirectURL;
#endif
        m_pendingDownloads.remove(originalURL);
        m_pendingDownloads.insert(redirectURL);

        QNetworkRequest request;
        request.setUrl(redirectURL);
        m_networkManager->get(request);
    }
    else
    {
        m_pendingDownloads.remove(originalURL);
        emit downloadFailure(originalURL);
        emit pendingDownloadCountChanged();
        reply->deleteLater();
    }
}

void InternetRadio::onHttpFinished(QNetworkReply * reply) {
    QUrl originalURL = reply->request().url();
    if (reply->error() == QNetworkReply::NoError)
    {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
#ifdef VERBOSE_LOGGING
        qDebug() << "URL=" << originalURL;
        qDebug() << "HTTP status=" << statusCode;
#endif
        switch(statusCode.toInt())
        {
        case 302:
            handleRedirect(reply);
            return;
        case 200:
            finishDownload(reply);
            return;
        }
    }
    else
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "Could not download" << originalURL;
#endif
        m_pendingDownloads.remove(originalURL);
        reply->deleteLater();
        emit downloadFailure(originalURL);
        emit pendingDownloadCountChanged();
    }
}

void InternetRadio::finishDownload(QNetworkReply * reply) {
    QUrl originalURL = reply->request().url();
#ifdef VERBOSE_LOGGING
    qDebug() << "Reading reply from" << originalURL << "(bytes=" << reply->size() << ")";
#endif
    if(m_pendingDownloads.contains(originalURL))
    {
        m_pendingDownloads.remove(originalURL);
        emit pendingDownloadCountChanged();
    }
    else
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "Could not match download URL " << originalURL;
#endif
        reply->deleteLater();
        m_pendingDownloads.remove(originalURL);
        emit downloadFailure(originalURL);
        emit pendingDownloadCountChanged();
        return;
    }

    QByteArray data = reply->readAll();
    reply->deleteLater();

    QString playlist(data);
    QRegExp rx("(http://[^\\s\\n]+)", Qt::CaseInsensitive, QRegExp::RegExp);
    QStringList result;
    int pos = 0;
    while ((pos = rx.indexIn(playlist, pos)) != -1) {
        QString url = rx.cap(1);
        if(url.endsWith(QChar('/'))) {
            url += ";";
        }
        result << url;
        pos += rx.matchedLength();
    }

    emit downloadFinished(originalURL, result);
}

int InternetRadio::pendingDownloadCount() const {
    return m_pendingDownloads.size();
}

QSet<QUrl> const& InternetRadio::pendingDownloads() const {
    return m_pendingDownloads;
}
