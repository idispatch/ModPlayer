#include "Proximity.hpp"
#include <QtCore/QDebug>

Proximity::Proximity(QObject *parent)
    : QObject(parent),
      m_close(false)
{
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
            emit closeProximity();
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
            m_sensor.start();
        } else {
            m_sensor.stop();
        }
        emit activeChanged();
    }
}

