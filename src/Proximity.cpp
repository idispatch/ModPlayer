#include "Proximity.hpp"
#include <bb/cascades/application.h>
#include <QtCore/QDebug>

namespace {
    const int PROXIMITY_TIMEOUT_MS = 2000;
}

Proximity::Proximity(QObject *parent)
    : QObject(parent),
      m_close(false)
{
    m_lastProximity = QDateTime::currentDateTime().addSecs(3);
    if(!m_sensor.isConnectedToBackend()) {
        if (!m_sensor.connectToBackend()) {
            qDebug() << "Cannot connect to proximity sensor backend!";
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

void Proximity::onReadingChanged() {
    QtMobility::QProximityReading *reading = m_sensor.reading();
    if (m_close != reading->close()) {
        m_close = reading->close();
        if (m_close) {
            QDateTime now = QDateTime::currentDateTime();
            qDebug() << "Proximity detected:" << now;
            if(m_lastProximity.msecsTo(now) > PROXIMITY_TIMEOUT_MS) {
                m_lastProximity = now;
                emit closeProximity();
            }
        }
    }
}

bool Proximity::active() const {
    return m_sensor.isActive();
}

void Proximity::setActive(bool value) {
    if(active() != value) {
        if (value) {
            m_lastProximity = QDateTime::currentDateTime().addSecs(3);
            if(m_sensor.isConnectedToBackend()) {
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

void Proximity::enableSensor() {
    setActive(true);
}

void Proximity::disableSensor() {
    setActive(false);
}

