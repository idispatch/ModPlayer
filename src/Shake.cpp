#include "Shake.hpp"
#include <QtCore/QDebug>
#include <math.h>

namespace {
    const double SHAKE_SENSITIVITY = 1.7;
    const qint64 SHAKE_TIMEOUT_MS  = 1500; // milliseconds
}

Shake::Shake(QObject *parent)
    : QObject(parent),
      m_accel(0.0),
      m_accelLast(0.0),
      m_accelCurrent(0.0)
{
    if(!m_sensor.isConnectedToBackend()) {
        if (!m_sensor.connectToBackend()) {
            qDebug() << "Cannot connect to shake sensor backend!";
        }
        m_sensor.setAxesOrientationMode(QtMobility::QAccelerometer::FixedOrientation);
        m_sensor.setAccelerationMode(QtMobility::QAccelerometer::User);
    }
    m_sensor.addFilter(this);
    m_lastShake = QDateTime::currentDateTime();
}

bool Shake::filter(QtMobility::QAccelerometerReading *reading) {
    const double x = reading->x();
    const double y = reading->y();
    const double z = reading->z();

    m_accelLast = m_accelCurrent;
    m_accelCurrent = sqrt(x * x  + y * y + z * z);
    m_accel = m_accel * 0.9 + (m_accelCurrent - m_accelLast) * 0.1;

    if(m_accel > SHAKE_SENSITIVITY) {
        QDateTime now = QDateTime::currentDateTime();
        if(m_lastShake.msecsTo(now) > SHAKE_TIMEOUT_MS) {
            qDebug() << "Shake detected:" << m_accel;
            emit shakeDetected();
            m_lastShake = now;
        }
    }

    // Do no further processing of the sensor data
    return false;
}

bool Shake::active() const {
    return m_sensor.isActive();
}

void Shake::setActive(bool value) {
    if(active() != value) {
        if (value) {
            m_accel = 0.0;
            m_accelLast = 0.0;
            m_accelCurrent = 0.0;
            m_sensor.setSkipDuplicates(true);
            m_sensor.setAlwaysOn(true);
            m_sensor.start();
        } else {
            m_sensor.stop();
        }
        emit activeChanged();
    }
}

