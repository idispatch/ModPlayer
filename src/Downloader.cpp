#include "Downloader.hpp"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QList>
#include <QByteArray>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

const int Downloader::InvalidModuleId = -1;

Downloader::Downloader(QObject * parent)
    : QObject(parent),
      m_networkManager(new QNetworkAccessManager(this)) {
    bool rc;
    rc = connect(m_networkManager,
                 SIGNAL(finished(QNetworkReply*)),
                 this,
                 SLOT(onHttpFinished(QNetworkReply*)));
    Q_ASSERT(rc);

    rc = connect(m_networkManager,
                 SIGNAL(networkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)),
                 this,
                 SLOT(onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility)));
    Q_ASSERT(rc);
    Q_UNUSED(rc);
}

void Downloader::download(int modId) {
    QString url = QString("http://modarchive.org/data/downloads.php?moduleid=%1").arg(modId);
    if(m_pendingDownloads.contains(url)) {
        qDebug() << "Already downloading" << url;
        return;
    }

    m_pendingDownloads.insert(url, modId);
    qDebug() << "Starting downloading" << url;

    QNetworkRequest request;
    request.setUrl(QUrl(url));
    m_networkManager->get(request);

    emit downloadStarted(modId);
    emit pendingDownloadCountChanged();
}

void Downloader::onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility a) {
    qDebug() << "Network accessible changed:" << a;
    if(a != QNetworkAccessManager::Accessible) {
        QList<int> pending = m_pendingDownloads.values();
        QList<int>::const_iterator i;
        for(i = pending.begin();
            i != pending.end();
            i++) {
            emit downloadFailure(*i);
        }
        m_pendingDownloads.clear();
        emit pendingDownloadCountChanged();
    }
}

void Downloader::handleRedirect(QNetworkReply * reply) {
    QVariant redirect = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    QUrl redirectURL = redirect.toUrl();
    QUrl originalURL = reply->request().url();

    if(!redirectURL.isEmpty() && redirectURL != originalURL) {
        qDebug() << "Redirected from" << originalURL << "to" << redirectURL;

        int modId = m_pendingDownloads.value(originalURL, InvalidModuleId);
        if(modId == InvalidModuleId) {
            emit downloadFailure(modId);
            reply->deleteLater();
        } else {
            m_pendingDownloads.remove(originalURL);
            m_pendingDownloads.insert(redirectURL, modId);

            QNetworkRequest request;
            request.setUrl(QUrl(redirectURL));
            m_networkManager->get(request);
        }
    } else {
        int modId = m_pendingDownloads.value(originalURL, InvalidModuleId);
        if(modId != InvalidModuleId) {
            m_pendingDownloads.remove(originalURL);
        }
        emit downloadFailure(modId);
        emit pendingDownloadCountChanged();
        reply->deleteLater();
    }
}

void Downloader::onHttpFinished(QNetworkReply * reply) {
    QUrl originalURL = reply->request().url();
    int modId = m_pendingDownloads.value(originalURL, InvalidModuleId);
    if(modId != InvalidModuleId) {
        qDebug() << "Received reply for module" << modId << "from" << originalURL;
    } else {
        qDebug() << "Could not match download URL " << originalURL;
        reply->deleteLater();
        emit downloadFailure(modId);
        return;
    }

    if (reply->error() == QNetworkReply::NoError) {
        QVariant statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
        qDebug() << "URL=" << reply->request().url();
        qDebug() << "HTTP status=" << statusCode;

        switch(statusCode.toInt()) {
        case 302:
            handleRedirect(reply);
            return;
        case 200:
            finishDownload(reply);
            return;
        }
    }

    qDebug() << "Could not download module" << modId << "from" << originalURL;
    m_pendingDownloads.remove(originalURL);
    reply->deleteLater();
    emit downloadFailure(modId);
    emit pendingDownloadCountChanged();
}

void Downloader::finishDownload(QNetworkReply * reply) {
    QUrl originalURL = reply->request().url();
    qDebug() << "Reading reply from" << originalURL << "(bytes=" << reply->size() << ")";

    int modId = InvalidModuleId;
    if(m_pendingDownloads.contains(originalURL)) {
        modId = m_pendingDownloads[originalURL];
        m_pendingDownloads.remove(originalURL);
        emit pendingDownloadCountChanged();
    } else {
        qDebug() << "Could not match download URL " << originalURL;
        reply->deleteLater();
        m_pendingDownloads.remove(originalURL);
        emit downloadFailure(modId);
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
    if(file.exists()) {
        qDebug() << "File" << diskPath << "already exists";
        if(file.remove()) {
            qDebug() << "Deleted file" << diskPath;
        } else {
            qDebug() << "Failed to delete file" << diskPath;
            emit downloadFailure(modId);
        }
    }

    if(file.open(QIODevice::WriteOnly)) {
        qint64 writtenBytes = file.write(data);
        qDebug() << "Written bytes:" << writtenBytes;
        file.close();
        emit downloadFinished(file.fileName());
    } else {
        qDebug() << "Could not open file" << file.fileName();
        emit downloadFailure(modId);
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
        << "(Downloader: pending downloads="
        << c.pendingDownloads()
        << ")";
    return dbg.space();
}

