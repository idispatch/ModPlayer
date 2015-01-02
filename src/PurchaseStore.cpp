#include "PurchaseStore.hpp"
#include <QSettings>
#include <QStringList>

namespace {
QString const PURCHASE_KEY_NAME = "purchases/sku";
}

PurchaseStore::PurchaseStore(QSettings &settings, QObject* parent)
    : QObject(parent),
      m_store(settings) {
}

bool PurchaseStore::isPurchased(QString const& sku) {
    bool result;
    QStringList purchases = m_store.value(PURCHASE_KEY_NAME, QStringList()).toStringList();
    result = purchases.contains(sku);
    return result;
}

void PurchaseStore::storePurchase(QString const& sku) {
    QStringList purchases = m_store.value(PURCHASE_KEY_NAME, QStringList()).toStringList();
    if (!purchases.contains(sku)) {
        purchases.append(sku);
        m_store.setValue(PURCHASE_KEY_NAME, purchases);
    }
    emit purchaseRetrieved(sku);
}

void PurchaseStore::retrieveLocalPurchases() {
    QStringList purchases = m_store.value(PURCHASE_KEY_NAME).toStringList();
    if (!purchases.isEmpty()) {
        foreach(QString sku, purchases) {
            emit purchaseRetrieved(sku);
        }
    }
}
