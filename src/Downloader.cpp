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
        qDebug() << "Canceling" << pending.size() << "downloads";
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
            qDebug() << "Already downloading" << url;
        }
        else
        {
            m_pendingDownloads.insert(url, id);
            //qDebug() << "Starting downloading" << url;

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
        //qDebug() << "Redirected from" << originalURL << "to" << redirectURL;
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
        qDebug() << "Received reply for module" << id << "from" << originalURL;
    }
    else
    {
        qDebug() << "Could not match download URL " << originalURL;
        reply->deleteLater();
        emit downloadFailure(id);
        return;
    }

    if (reply->error() == QNetworkReply::NoError)
    {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        //qDebug() << "URL=" << reply->request().url();
        //qDebug() << "HTTP status=" << statusCode;
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
        qDebug() << "Could not download module" << id << "from" << originalURL;
        m_pendingDownloads.remove(originalURL);
        reply->deleteLater();
        emit downloadFailure(id);
        emit pendingDownloadCountChanged();
    }
}

void Downloader::finishDownload(QNetworkReply * reply) {
    QUrl originalURL = reply->request().url();
    //qDebug() << "Reading reply from" << originalURL << "(bytes=" << reply->size() << ")";

    int id = InvalidModuleId;
    if(m_pendingDownloads.contains(originalURL))
    {
        id = m_pendingDownloads[originalURL];
        m_pendingDownloads.remove(originalURL);
        emit pendingDownloadCountChanged();
    }
    else
    {
        qDebug() << "Could not match download URL " << originalURL;
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
    qDebug() << "Saving file" << diskPath;

    QFile file(diskPath);
    if(file.exists())
    {
        qDebug() << "File" << diskPath << "already exists";
        if(file.remove())
        {
            qDebug() << "Deleted file" << diskPath;
        }
        else
        {
            qDebug() << "Failed to delete file" << diskPath;
            emit downloadFailure(id);
        }
    }

    if(file.open(QIODevice::WriteOnly))
    {
        qint64 writtenBytes = file.write(data);
        qDebug() << "Written bytes:" << writtenBytes;
        file.close();
        emit downloadFinished(file.fileName());
    }
    else
    {
        qDebug() << "Could not open file" << file.fileName();
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

