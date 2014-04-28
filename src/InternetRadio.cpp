#include "InternetRadio.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QList>
#include <QByteArray>
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
    qDebug() << "Network accessible changed to" << a;
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
        qDebug() << "Could not download module" << originalURL;
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

    QString fileName = reply->request().url().toString(QUrl::None);
    int pos = fileName.lastIndexOf('/');
    fileName = fileName.mid(pos);

    QString diskPath = QDir::tempPath() + fileName;
#ifdef VERBOSE_LOGGING
    qDebug() << "Saving file" << diskPath;
#endif
    QFile file(diskPath);
    if(file.exists())
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "File" << diskPath << "already exists";
#endif
        if(file.remove())
        {
#ifdef VERBOSE_LOGGING
            qDebug() << "Deleted file" << diskPath;
#endif
        }
        else
        {
#ifdef VERBOSE_LOGGING
            qDebug() << "Failed to delete file" << diskPath;
#endif
            emit downloadFailure(originalURL);
        }
    }

    if(file.open(QIODevice::WriteOnly))
    {


#ifdef VERBOSE_LOGGING
        qint64 writtenBytes = file.write(data);
        qDebug() << "Written bytes:" << writtenBytes;
#else
        file.write(data);
#endif
        file.close();
        emit downloadFinished(file.fileName());
    }
    else
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "Could not open file" << file.fileName();
#endif
        emit downloadFailure(originalURL);
    }
}

int InternetRadio::pendingDownloadCount() const {
    return m_pendingDownloads.size();
}

QSet<QUrl> const& InternetRadio::pendingDownloads() const {
    return m_pendingDownloads;
}
