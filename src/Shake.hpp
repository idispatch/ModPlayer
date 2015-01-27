#ifndef SHAKE_HPP
#define SHAKE_HPP

#include <QObject>
#include <QtCore/QDateTime>
#include <QtSensors/QAccelerometer>

class Shake : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Shake)
public:
    Shake(QObject *parent = 0);

    bool active() const;
    void setActive(bool active);
Q_SIGNALS:
    void activeChanged();
    void shakeDetected();
private slots:
    void onReadingChanged();
    void enableSensor();
    void disableSensor();
private:
    void enableSensor();
    void disableSensor();
private:
    QtMobility::QAccelerometer m_sensor;
    QDateTime                  m_lastShake;
    double                     m_accel;
    double                     m_accelLast;
    double                     m_accelCurrent;
};

#endif
