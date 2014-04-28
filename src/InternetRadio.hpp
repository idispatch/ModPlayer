#ifndef INTERNETRADIO_HPP_
#define INTERNETRADIO_HPP_

#include <QDebug>
#include <QObject>
#include <QSet>
#include <QUrl>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkConfigurationManager>
#include "InstanceCounter.hpp"

class QNetworkReply;

class InternetRadio : public QObject,
                      public InstanceCounter<InternetRadio> {
    Q_OBJECT
public:
    InternetRadio(QObject *Player);
    ~InternetRadio();
    Q_INVOKABLE void download(QUrl const& url);

    int pendingDownloadCount() const;
    QSet<QUrl> const& pendingDownloads() const;

    using InstanceCounter<InternetRadio>::getInstanceCount;
    using InstanceCounter<InternetRadio>::getMaxInstanceCount;
Q_SIGNALS:
    void pendingDownloadCountChanged();
    void downloadStarted(QUrl const& url);
    void downloadFinished(QUrl const& url);
    void downloadFailure(QUrl const& url);

private slots:
    void onHttpFinished(QNetworkReply * reply);
    void onNetworkAccessibleChanged(QNetworkAccessManager::NetworkAccessibility a);
    void onOnlineStateChanged(bool isOnline);
private:
    Q_DISABLE_COPY(InternetRadio)
    void handleRedirect(QNetworkReply * reply);
    void finishDownload(QNetworkReply * reply);
    void openNetworkConfiguration();
    bool isNetworkAvailable() const;
    void cancelAllPendingDownloads();
private:
    QNetworkAccessManager * m_networkManager;
    QNetworkConfigurationManager * m_networkConfigurationManager;
    QSet<QUrl> m_pendingDownloads;
};

Q_DECLARE_METATYPE(InternetRadio*);

#endif /* INTERNETRADIO_HPP_ */
