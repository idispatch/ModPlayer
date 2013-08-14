#ifndef DOWNLOADER_HPP_
#define DOWNLOADER_HPP_

#include <QDebug>
#include <QObject>
#include <QMap>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfigurationManager>

class QNetworkReply;

class Downloader : public QObject {
    Q_OBJECT
public:
    Downloader(QObject *Player = 0);

    Q_PROPERTY(int pendingDownloadCount READ pendingDownloadCount NOTIFY pendingDownloadCountChanged FINAL)

    Q_INVOKABLE void download(int modId);

    int pendingDownloadCount() const;
    QMap<QUrl, int> const& pendingDownloads() const;
Q_SIGNALS:
    void pendingDownloadCountChanged();
    void downloadStarted(int modId);
    void downloadFinished(QString fileName);
    void downloadFailure(int modId);

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
