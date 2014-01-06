#include "Downloader.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QList>
#include <QByteArray>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <bb/system/InvokeManager>

const int Downloader::InvalidModuleId = -1;

using namespace bb::system;

const QString Downloader::ModArchiveSite = QString("http://modarchive.org/data/downloads.php?moduleid=%1");

template<>
int InstanceCounter<Downloader>::s_count;
template<>
int InstanceCounter<Downloader>::s_maxCount;

Downloader::Downloader(QObject * parent)
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

Downloader::~Downloader() {
#ifdef VERBOSE_LOGGING
    qDebug() << "Downloader::~Downloader()";
#endif
}

void Downloader::openNetworkConfiguration()
{
    InvokeManager invokeManager;
    InvokeRequest request;
    request.setTarget("sys.settings.target");
    request.setAction("bb.action.OPEN");
    request.setMimeType("settings/view");
    request.setUri(QUrl("settings://networkconnections"));
    invokeManager.invoke(request);
}

bool Downloader::isNetworkAvailable() const {
    QNetworkConfigurationManager mgr;
    QList<QNetworkConfiguration> active = mgr.allConfigurations(QNetworkConfiguration::Active);
    return active.count();
}

void Downloader::cancelAllPendingDownloads() {
    QList<int> pending = m_pendingDownloads.values();
    if(pending.size() > 0)
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "Cancelling" << pending.size() << "downloads";
#endif
        QList<int>::const_iterator i;
        for(i = pending.begin(); i != pending.end(); ++i)
        {
            emit downloadFailure(*i);
        }
        m_pendingDownloads.clear();
        emit pendingDownloadCountChanged();
    }
}

void Downloader::onOnlineStateChanged(bool isOnline) {
    if(!isOnline)
    {
        cancelAllPendingDownloads();
    }
}

void Downloader::download(int id) {
    if(!isNetworkAvailable())
    {
        openNetworkConfiguration();
        emit downloadFailure(id);
    }
    else
    {
        QString url = ModArchiveSite.arg(id);
        if(m_pendingDownloads.contains(url)) {
#ifdef VERBOSE_LOGGING
            qDebug() << "Already downloading" << url;
#endif
        }
        else
        {
            m_pendingDownloads.insert(url, id);
#ifdef VERBOSE_LOGGING
            qDebug() << "Starting downloading" << url;
#endif
            QNetworkRequest request = QNetworkRequest(QUrl(url));
            m_networkManager->get(request);

            emit downloadStarted(id);
            emit pendingDownloadCountChanged();
        }
    }
}

void Downloader::onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility a) {
    qDebug() << "Network accessible changed to" << a;
    if(a != QNetworkAccessManager::Accessible)
    {
        cancelAllPendingDownloads();
    }
}

void Downloader::handleRedirect(QNetworkReply * reply) {
    QVariant redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    QUrl redirectURL = redirect.toUrl();
    QUrl originalURL = reply->request().url();

    if(!redirectURL.isEmpty() && redirectURL != originalURL)
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "Redirected from" << originalURL << "to" << redirectURL;
#endif
        int id = m_pendingDownloads.value(originalURL, InvalidModuleId);
        if(id == InvalidModuleId)
        {
            emit downloadFailure(id);
            reply->deleteLater();
        }
        else
        {
            m_pendingDownloads.remove(originalURL);
            m_pendingDownloads.insert(redirectURL, id);

            QNetworkRequest request;
            request.setUrl(QUrl(redirectURL));
            m_networkManager->get(request);
        }
    }
    else
    {
        int id = m_pendingDownloads.value(originalURL, InvalidModuleId);
        if(id != InvalidModuleId)
        {
            m_pendingDownloads.remove(originalURL);
        }
        emit downloadFailure(id);
        emit pendingDownloadCountChanged();
        reply->deleteLater();
    }
}

void Downloader::onHttpFinished(QNetworkReply * reply) {
    QUrl originalURL = reply->request().url();

    int id = m_pendingDownloads.value(originalURL, InvalidModuleId);
    if(id != InvalidModuleId)
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "Received reply for module" << id << "from" << originalURL;
#endif
    }
    else
    {
#ifdef VERBOSE_LOGGING
        qDebug() << "Could not match download URL " << originalURL;
#endif
        reply->deleteLater();
        emit downloadFailure(id);
        return;
    }

    if (reply->error() == QNetworkReply::NoError)
    {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
#ifdef VERBOSE_LOGGING
        qDebug() << "URL=" << reply->request().url();
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
        qDebug() << "Could not download module" << id << "from" << originalURL;
#endif
        m_pendingDownloads.remove(originalURL);
        reply->deleteLater();
        emit downloadFailure(id);
        emit pendingDownloadCountChanged();
    }
}

void Downloader::finishDownload(QNetworkReply * reply) {
    QUrl originalURL = reply->request().url();
#ifdef VERBOSE_LOGGING
    qDebug() << "Reading reply from" << originalURL << "(bytes=" << reply->size() << ")";
#endif
    int id = InvalidModuleId;
    if(m_pendingDownloads.contains(originalURL))
    {
        id = m_pendingDownloads[originalURL];
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
        emit downloadFailure(id);
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
            emit downloadFailure(id);
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
        emit downloadFailure(id);
    }
}

int Downloader::pendingDownloadCount() const {
    return m_pendingDownloads.size();
}

QMap<QUrl, int> const& Downloader::pendingDownloads() const {
    return m_pendingDownloads;
}

QDebug operator << (QDebug dbg, Downloader const &c) {
    dbg.nospace()
        << "(Downloader: pending="
        << c.pendingDownloads()
        << ")";
    return dbg.space();
}

