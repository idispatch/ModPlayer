#ifndef DOWNLOADER_HPP_
#define DOWNLOADER_HPP_

#include <QDebug>
#include <QObject>
#include <QMap>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfigurationManager>
#include "InstanceCounter.hpp"

class QNetworkReply;

class Downloader : public QObject,
                   public InstanceCounter<Downloader> {
    Q_OBJECT
public:
    Downloader(QObject *Player = 0);

    Q_PROPERTY(int pendingDownloadCount READ pendingDownloadCount NOTIFY pendingDownloadCountChanged FINAL)

    Q_INVOKABLE void download(int id);

    int pendingDownloadCount() const;
    QMap<QUrl, int> const& pendingDownloads() const;

    using InstanceCounter<Downloader>::getInstanceCount;
    using InstanceCounter<Downloader>::getMaxInstanceCount;
Q_SIGNALS:
    void pendingDownloadCountChanged();
    void downloadStarted(int id);
    void downloadFinished(QString fileName);
    void downloadFailure(int id);

private slots:
    void onHttpFinished(QNetworkReply * reply);
    void onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility a);
    void onOnlineStateChanged(bool isOnline);
private:
    Q_DISABLE_COPY(Downloader)
    void handleRedirect(QNetworkReply * reply);
    void finishDownload(QNetworkReply * reply);
    void openNetworkConfiguration();
    bool isNetworkAvailable() const;
    void cancelAllPendingDownloads();
private:
    static const QString ModArchiveSite;
    QNetworkAccessManager * m_networkManager;
    QNetworkConfigurationManager * m_networkConfigurationManager;
    QMap<QUrl, int> m_pendingDownloads;
    static const int InvalidModuleId;
};

Q_DECLARE_METATYPE(Downloader*);

QDebug operator << (QDebug dbg, Downloader const &d);

#endif
