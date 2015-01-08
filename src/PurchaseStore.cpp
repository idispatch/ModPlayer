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
    QString const PURCHASEID_KEY_NAME = "purchases/purchaseId";
    QString const PURCHASEDATE_KEY_NAME = "purchases/purchaseDate";
    QString const FEATURE_NAME = "ModPlayerPlus";
    QString const PURCHASE_FILE_NAME = ".purchase";
}

PurchaseStore::PurchaseStore(QSettings &settings, QObject* parent)
    : QObject(parent),
      m_store(settings),
      m_updatingStatus(false),
      m_reloadingStatus(false) {
    int rc;
    Q_UNUSED(rc);

    rc = QObject::connect(&m_manager, SIGNAL(purchaseFinished(bb::platform::PurchaseReply*)),
                          this, SLOT(onPurchaseFinished(bb::platform::PurchaseReply*)));
    Q_ASSERT(rc);

    rc = QObject::connect(&m_manager, SIGNAL(existingPurchasesFinished(bb::platform::ExistingPurchasesReply*)),
                          this, SLOT(onExistingPurchasesFinished(bb::platform::ExistingPurchasesReply*)));
    Q_ASSERT(rc);

    //m_manager.setConnectionMode(bb::platform::PaymentConnectionMode::Test);
    m_manager.setConnectionMode(bb::platform::PaymentConnectionMode::Production);
}

void PurchaseStore::buy() {
    using namespace bb::device;

    HardwareInfo hwInfo;

    QString metadata;
    {
        QTextStream stream;
        stream.setString(&metadata);
        stream << "IMEI: " << hwInfo.imei() << "; ";
        stream << "PIN: " << hwInfo.pin() << "; ";
        stream << "DEVICENAME: " << hwInfo.deviceName() << "; ";
        stream << "HARDWAREID: " << hwInfo.hardwareId() << "; ";
        stream << "MODELNAME: " << hwInfo.modelName() << "; ";
        stream << "MODELNUMBER: " << hwInfo.modelNumber() << "; ";
        stream << "SERIALNUMBER: " << hwInfo.serialNumber() << ";";
    }

    QVariantMap variableMap;
    variableMap["IMEI"] = hwInfo.imei();
    variableMap["PIN"] = hwInfo.pin();
    variableMap["DEVICENAME"] = hwInfo.deviceName();
    variableMap["HARDWAREID"] = hwInfo.hardwareId();
    variableMap["MODELNAME"] = hwInfo.modelName();
    variableMap["MODELNUMBER"] = hwInfo.modelNumber();
    variableMap["SERIALNUMBER"] = hwInfo.serialNumber();

    m_manager.requestPurchase("", FEATURE_NAME, "", metadata, variableMap);
}

void PurchaseStore::onExistingPurchasesFinished(bb::platform::ExistingPurchasesReply *reply) {
    qDebug() << "=== Update existing purchases finished ===";

    m_updatingStatus = false;

    bool isPurchased = false;

    using namespace bb::platform;
    if(reply != NULL && reply->isFinished() && !reply->isError()) {
        const QList<PurchaseReceipt> purchaseReceipts = reply->purchases();
        const int numPurchases = purchaseReceipts.size();
        for(int i = 0; i < numPurchases; i++) {
            PurchaseReceipt const& receipt = purchaseReceipts[i];
            if(receipt.isValid()) {
                if(receipt.state() == DigitalGoodState::Owned) {
                    savePurchase(receipt.digitalGoodSku());
                    isPurchased = true;
                }
            }
        }
    }

    if(isPurchased) {
        qDebug() << "=== ModPlayer Plus is purchased ===";
    } else {
        qDebug() << "=== ModPlayer Plus is not purchased ===";
    }

    if(m_reloadingStatus) {
        m_reloadingStatus = false;
        emit remoteStatusRetrieved();
    }

    emit updatingStatusChanged();
}

void PurchaseStore::onPurchaseFinished(bb::platform::PurchaseReply *reply) {
    using namespace bb::platform;
    if(reply != NULL && reply->isFinished()) {
        if(reply->isError()) {
            qDebug() << "=========== Purchase error ===========";
            qDebug() << reply->errorText();
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
                    info += "PURCHASEID: " + receipt.purchaseId() + "; ";
                    info += "DATE: " + receipt.date().toString(Qt::ISODate) + "; ";
                    info += "DATA: [" + reply->purchaseMetadata() + "]";
                    qDebug() << "=========== Purchase succeeded ===========";
                    qDebug() << info;
                    emit purchaseSucceeded(info);
                } else {
                    qDebug() << "Invalid SKU in purchase";
                    emit purchaseFailed("Purchased invalid SKU");
                }
            } else {
                qDebug() << "Invalid SKU state in purchase";
                emit purchaseFailed("Invalid SKU state");
            }
        }
    } else {
        qDebug() << "Invalid purchase reply";
    }
}

void PurchaseStore::updatePurchaseMetadata(QString const& purchaseId,
                                           QDateTime datetime,
                                           QString const& metadata,
                                           QVariantMap parameters) {
    Q_UNUSED(parameters);
    QFile file(FileUtils::joinPath(QDir::homePath(), PURCHASE_FILE_NAME));
    if(file.open(QFile::WriteOnly | QFile::Truncate)) {
        QTextStream stream(&file);
        stream << "PURCHASEID: " << purchaseId << ";\n";
        stream << "DATE: " << datetime.toString(Qt::ISODate) << ";\n";
        stream << "DATA: [" << metadata << "]\n";
        file.flush();
        file.close();
    }

    m_store.setValue(PURCHASEDATE_KEY_NAME, datetime.toString(Qt::ISODate));
    m_store.setValue(PURCHASEID_KEY_NAME, purchaseId);
    m_store.sync();
}

bool PurchaseStore::purchased() const {
    bool result;
    const QStringList purchases = m_store.value(PURCHASE_KEY_NAME, QStringList()).toStringList();
    result = purchases.contains(FEATURE_NAME);
    if(!result){
        result = QFile(FileUtils::joinPath(QDir::homePath(), PURCHASE_FILE_NAME)).exists();
    }
    return result;
}

bool PurchaseStore::updatingStatus() const {
    return m_updatingStatus;
}

void PurchaseStore::savePurchase(QString const& sku) {
    QStringList purchases = m_store.value(PURCHASE_KEY_NAME, QStringList()).toStringList();
    if (!purchases.contains(sku)) {
        purchases.append(sku);
        m_store.setValue(PURCHASE_KEY_NAME, purchases);
        m_store.sync();
    }
    if(sku == FEATURE_NAME) {
        emit purchasedChanged();
    }
}

void PurchaseStore::loadPurchasesFromStore() {
    if(!m_updatingStatus) {
        m_updatingStatus = true;
        emit updatingStatusChanged();
        m_manager.requestExistingPurchases(false);
    }
}

void PurchaseStore::reloadPurchasesFromStore() {
    if(!m_updatingStatus) {
        m_updatingStatus  = true;
        m_reloadingStatus = true;
        emit updatingStatusChanged();
        m_manager.requestExistingPurchases(true);
    }
}

void PurchaseStore::loadLocalPurchases() {
    QStringList purchases = m_store.value(PURCHASE_KEY_NAME).toStringList();
    if (!purchases.isEmpty()) {
        foreach(QString sku, purchases) {
            if(sku == FEATURE_NAME) {
                emit purchasedChanged();
            }
        }
    }
}
