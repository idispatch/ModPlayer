#include "Shake.hpp"
#include <bb/cascades/application.h>
#include <QtCore/QDebug>
#include <QtSensors/QAccelerometer>
#include <math.h>

namespace {
    const double SHAKE_SENSITIVITY = 1.7;
    const double SHAKE_FILTER      = 0.8;
    const qint64 SHAKE_TIMEOUT_MS  = 1500; // milliseconds
}

Shake::Shake(QObject *parent)
    : QObject(parent),
      m_accel(0.0),
      m_accelLast(0.0),
      m_accelCurrent(0.0)
{
    m_lastShake = QDateTime::currentDateTime().addSecs(3);
    if(!m_sensor.isConnectedToBackend()) {
        if (!m_sensor.connectToBackend()) {
            qDebug() << "Cannot connect to shake sensor backend!";
        } else {
            bool rc = QObject::connect(&m_sensor,
                                       SIGNAL(readingChanged()),
                                       this,
                                       SLOT(onReadingChanged()));
            Q_ASSERT(rc);

            rc = QObject::connect(bb::cascades::Application::instance(),
                          SIGNAL(fullscreen()),
                          this,
                          SLOT(enableSensor()));
            Q_ASSERT(rc);

            rc = QObject::connect(bb::cascades::Application::instance(),
                                  SIGNAL(thumbnail()),
                                  this,
                                  SLOT(disableSensor()));
            Q_ASSERT(rc);

            rc = QObject::connect(bb::cascades::Application::instance(),
                                  SIGNAL(invisible()),
                                  this,
                                  SLOT(disableSensor()));
            Q_ASSERT(rc);

            rc = QObject::connect(bb::cascades::Application::instance(),
                                  SIGNAL(asleep()),
                                  this,
                                  SLOT(disableSensor()));
            Q_ASSERT(rc);

            Q_UNUSED(rc);
        }
    }
}

void Shake::onReadingChanged() {
    QtMobility::QAccelerometerReading *reading = m_sensor.reading();

    const double x = reading->x();
    const double y = reading->y();
    const double z = reading->z();

    m_accelLast = m_accelCurrent;
    m_accelCurrent = sqrt(x * x  + y * y + z * z);
    m_accel = m_accel * SHAKE_FILTER +
              (m_accelCurrent - m_accelLast) * (1 - SHAKE_FILTER);

    if(m_accel > SHAKE_SENSITIVITY) {
        QDateTime now = QDateTime::currentDateTime();
        if(m_lastShake.msecsTo(now) > SHAKE_TIMEOUT_MS) {
            qDebug() << "Shake detected:" << m_accel;
            emit shakeDetected();
            m_lastShake = now;
        }
    }
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
            m_lastShake = QDateTime::currentDateTime().addSecs(3);
            if(m_sensor.isConnectedToBackend()) {
                m_sensor.setAxesOrientationMode(QtMobility::QAccelerometer::FixedOrientation);
                m_sensor.setAccelerationMode(QtMobility::QAccelerometer::User);
                m_sensor.setSkipDuplicates(true);
                m_sensor.setAlwaysOn(true);
                m_sensor.start();
            }
        } else {
            if(m_sensor.isConnectedToBackend()) {
                m_sensor.stop();
            }
        }
        emit activeChanged();
    }
}

void Shake::enableSensor() {
    setActive(true);
}

void Shake::disableSensor() {
    setActive(false);
}
