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

    Q_PROPERTY(bool isPurchased READ isPurchased NOTIFY isPurchasedChanged FINAL)
public:
    PurchaseStore(QSettings &settings, QObject* parent = 0);

    bool isPurchased();

    Q_INVOKABLE void buy();

    Q_INVOKABLE void loadPurchasesFromStore();
    Q_INVOKABLE void reloadPurchasesFromStore();
    Q_INVOKABLE void loadLocalPurchases();

Q_SIGNALS:
    void isPurchasedChanged();
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
    bb::platform::PaymentManager m_manager;
};

Q_DECLARE_METATYPE(PurchaseStore*);

#endif
