#include "PurchaseStore.hpp"
#include <QSettings>
#include <QStringList>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <bb/platform/PurchaseReply>
#include <bb/platform/PurchaseReceipt>
#include <bb/platform/DigitalGoodState>
#include <bb/platform/PaymentConnectionMode>
#include <bb/device/HardwareInfo>
#include "FileUtils.hpp"

namespace {
QString const PURCHASE_KEY_NAME = "purchases/sku";
QString const FEATURE_NAME = "ModPlayerPlus";
}

PurchaseStore::PurchaseStore(QSettings &settings, QObject* parent)
    : QObject(parent),
      m_store(settings) {
    int rc;
    Q_UNUSED(rc);

    rc = QObject::connect(&m_manager, SIGNAL(purchaseFinished(bb::platform::PurchaseReply*)),
                          this, SLOT(onPurchaseFinished(bb::platform::PurchaseReply*)));
    Q_ASSERT(rc);

    rc = QObject::connect(&m_manager, SIGNAL(existingPurchasesFinished(bb::platform::ExistingPurchasesReply*)),
                          this, SLOT(onExistingPurchasesFinished(bb::platform::ExistingPurchasesReply*)));
    Q_ASSERT(rc);

    m_manager.setConnectionMode(bb::platform::PaymentConnectionMode::Test);
    //m_manager.setConnectionMode(bb::platform::PaymentConnectionMode::Production);
}

void PurchaseStore::buy() {
    using namespace bb::device;

    HardwareInfo hwInfo;

    QString metadata;
    {
        QTextStream stream;
        stream.setString(&metadata);
        stream << "IMEI: " << hwInfo.imei() << ";";
        stream << "PIN: " << hwInfo.pin() << ";";
        stream << "DEVICENAME: " << hwInfo.deviceName() << ";";
        stream << "HARDWAREID: " << hwInfo.hardwareId() << ";";
        stream << "MODELNAME: " << hwInfo.modelName() << ";";
        stream << "MODELNUMBER: " << hwInfo.modelNumber() << ";";
        stream << "SERIALNUMBER: " << hwInfo.serialNumber() << ";";
    }

    QVariantMap varMap;
    varMap["IMEI"] = hwInfo.imei();
    varMap["PIN"] = hwInfo.pin();
    varMap["DEVICENAME"] = hwInfo.deviceName();
    varMap["HARDWAREID"] = hwInfo.hardwareId();
    varMap["MODELNAME"] = hwInfo.modelName();
    varMap["MODELNUMBER"] = hwInfo.modelNumber();
    varMap["SERIALNUMBER"] = hwInfo.serialNumber();

    m_manager.requestPurchase("", FEATURE_NAME, "", metadata, varMap);
}

void PurchaseStore::onExistingPurchasesFinished(bb::platform::ExistingPurchasesReply *reply) {
    using namespace bb::platform;
    if(reply != NULL && reply->isFinished() && !reply->isError()) {
        QList<PurchaseReceipt> purchaseReceipts = reply->purchases();
        const int numPurchases = purchaseReceipts.size();
        for(int i = 0; i < numPurchases; i++) {
            PurchaseReceipt const& receipt = purchaseReceipts[i];
            if(receipt.isValid()) {
                if(receipt.state() == DigitalGoodState::Owned) {
                    savePurchase(receipt.digitalGoodSku());
                }
            }
        }
    }
}

void PurchaseStore::onPurchaseFinished(bb::platform::PurchaseReply *reply) {
    using namespace bb::platform;
    if(reply != NULL && reply->isFinished()) {
        if(reply->isError()) {
            emit purchaseFailed(reply->errorText());
        } else {
            const QString sku = reply->digitalGoodSku();
            const PurchaseReceipt receipt = reply->receipt();
            if(receipt.state() == DigitalGoodState::Owned) {
                savePurchase(sku);
                if(sku == FEATURE_NAME) {
                    updatePurchaseMetadata(receipt.purchaseId(),
                                           receipt.date(),
                                           reply->purchaseMetadata(),
                                           receipt.extraParameters());
                    QString info;
                    info += "PURCHASEID: " + receipt.purchaseId() + ";";
                    info += "DATE: " + receipt.date().toString(Qt::ISODate) + ";";
                    info += "DATA: [" + reply->purchaseMetadata() + "]";
                    emit purchaseSucceeded(info);
                } else {
                    emit purchaseFailed("Purchased invalid SKU");
                }
            } else {
                emit purchaseFailed("Invalid SKU state");
            }
        }
    }
}

void PurchaseStore::updatePurchaseMetadata(QString const& purchaseId,
                                           QDateTime datetime,
                                           QString const& metadata,
                                           QVariantMap parameters) {
    Q_UNUSED(parameters);
    QFile file(FileUtils::joinPath(QDir::homePath(), ".purchase"));
    if(file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream stream(&file);
        stream << "PURCHASEID: " << purchaseId << ";\n";
        stream << "DATE: " << datetime.toString(Qt::ISODate) << ";\n";
        stream << "DATA: [" << metadata << "]\n";
        file.flush();
        file.close();
    }
}

bool PurchaseStore::isPurchased() {
    bool result;
    QStringList purchases = m_store.value(PURCHASE_KEY_NAME, QStringList()).toStringList();
    result = purchases.contains(FEATURE_NAME);
    return result;
}

void PurchaseStore::savePurchase(QString const& sku) {
    QStringList purchases = m_store.value(PURCHASE_KEY_NAME, QStringList()).toStringList();
    if (!purchases.contains(sku)) {
        purchases.append(sku);
        m_store.setValue(PURCHASE_KEY_NAME, purchases);
        m_store.sync();
    }
    if(sku == FEATURE_NAME) {
        emit isPurchasedChanged();
    }
}

void PurchaseStore::loadPurchasesFromStore() {
    m_manager.requestExistingPurchases(false);
}

void PurchaseStore::reloadPurchasesFromStore() {
    m_manager.requestExistingPurchases(true);
}

void PurchaseStore::loadLocalPurchases() {
    QStringList purchases = m_store.value(PURCHASE_KEY_NAME).toStringList();
    if (!purchases.isEmpty()) {
        foreach(QString sku, purchases) {
            if(sku == FEATURE_NAME) {
                emit isPurchasedChanged();
            }
        }
    }
}
