#ifndef PURCHASESTORE_HPP_
#define PURCHASESTORE_HPP_

#include <QObject>
#include <QSettings>

class PurchaseStore : public QObject {
    Q_OBJECT
public:
    PurchaseStore(QSettings &settings, QObject* parent = 0);

    Q_INVOKABLE bool isPurchased(QString const& sku);
public Q_SLOTS:
    void deletePurchaseRecords();
    void storePurchase(QString const &sku);
    void retrieveLocalPurchases();
Q_SIGNALS:
    void purchaseRecordsDeleted();
    void purchaseRetrieved(QString const &sku);
private:
    QSettings &m_store;
};
#endif
