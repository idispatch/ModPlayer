#ifndef SHAKE_HPP
#define SHAKE_HPP

#include <QObject>
#include <QtCore/QDateTime>
#include <QtSensors/QAccelerometer>
#include <QtSensors/QAccelerometerFilter>

class Shake : public QObject,
              public QtMobility::QAccelerometerFilter
{
    Q_OBJECT
public:
    Shake(QObject *parent = 0);

    bool active() const;
    void setActive(bool active);
Q_SIGNALS:
    void activeChanged();
    void shakeDetected();
protected:
    bool filter(QtMobility::QAccelerometerReading *reading);
private:
    QtMobility::QAccelerometer m_sensor;
    QDateTime                  m_lastShake;
    double                     m_accel;
    double                     m_accelLast;
    double                     m_accelCurrent;
};

#endif
