#include "Proximity.hpp"
#include <QtCore/QDebug>

namespace {
    const int PROXIMITY_TIMEOUT_MS = 3000;
}

Proximity::Proximity(QObject *parent)
    : QObject(parent),
      m_close(false)
{
    m_lastProximity = QDateTime::currentDateTime().addSecs(3);
    if(!m_sensor.isConnectedToBackend()) {
        if (!m_sensor.connectToBackend()) {
            qDebug() << "Cannot connect to proximity sensor backend!";
        }
    }
    m_sensor.addFilter(this);
}

bool Proximity::filter(QtMobility::QProximityReading *reading) {
    if (m_close != reading->close()) {
        m_close = reading->close();
        if (m_close) {
            qDebug() << "Proximity detected";
            QDateTime now = QDateTime::currentDateTime();
            if(m_lastProximity.msecsTo(now) > PROXIMITY_TIMEOUT_MS) {
                m_lastProximity = now;
                emit closeProximity();
            }
        }
    }
    // Do no further processing of the sensor data
    return false;
}

bool Proximity::active() const {
    return m_sensor.isActive();
}

void Proximity::setActive(bool value) {
    if(active() != value) {
        if (value) {
            m_sensor.setSkipDuplicates(true);
            m_sensor.setAlwaysOn(true);
            m_lastProximity = QDateTime::currentDateTime().addSecs(3);
            m_sensor.start();
        } else {
            m_sensor.stop();
        }
        emit activeChanged();
    }
}

