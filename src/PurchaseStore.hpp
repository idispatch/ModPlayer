#ifndef PURCHASESTORE_HPP
#define PURCHASESTORE_HPP

#include <QObject>
#include <QMetaType>
#include <QSettings>
#include <QVariant>
#include <bb/platform/PaymentManager>
#include <bb/platform/PurchaseReply>
#include <bb/platform/ExistingPurchasesReply>

class PurchaseStore : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(PurchaseStore)

    Q_PROPERTY(bool purchased READ purchased NOTIFY purchasedChanged FINAL)
    Q_PROPERTY(bool updatingStatus READ updatingStatus NOTIFY updatingStatusChanged FINAL)
public:
    PurchaseStore(QSettings &settings, QObject* parent = 0);

    bool purchased() const;
    bool updatingStatus() const;

    Q_INVOKABLE void buy();

    Q_INVOKABLE void loadPurchasesFromStore();
    Q_INVOKABLE void reloadPurchasesFromStore();
    Q_INVOKABLE void loadLocalPurchases();

Q_SIGNALS:
    void remoteStatusRetrieved();
    void purchasedChanged();
    void updatingStatusChanged();
    void purchaseSucceeded(QString const& info);
    void purchaseFailed(QString const& reason);
private slots:
    void onPurchaseFinished(bb::platform::PurchaseReply*);
    void onExistingPurchasesFinished(bb::platform::ExistingPurchasesReply*);
private:
    void savePurchase(QString const& sku);
    void updatePurchaseMetadata(QString const& purchaseId,
                                QDateTime datetime,
                                QString const& metadata,
                                QVariantMap parameters);
private:
    QSettings &m_store;
    bool m_updatingStatus;
    bool m_reloadingStatus;
    bb::platform::PaymentManager m_manager;
};

Q_DECLARE_METATYPE(PurchaseStore*);

#endif
