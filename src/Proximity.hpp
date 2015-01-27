#ifndef PROXIMITY_HPP
#define PROXIMITY_HPP

#include <QObject>
#include <QtCore/QDateTime>
#include <QtSensors/QProximitySensor>
#include <QtSensors/QProximityReading>

class Proximity : public QObject,
                  public QtMobility::QProximityFilter
{
    Q_OBJECT
    Q_PROPERTY(bool active READ active WRITE setActive NOTIFY activeChanged)
public:
    Proximity(QObject *parent = 0);

    bool active() const;
    void setActive(bool active);
Q_SIGNALS:
    void activeChanged();
    void closeProximity();
protected:
    bool filter(QtMobility::QProximityReading *reading);
private:
    QtMobility::QProximitySensor m_sensor;
    bool                         m_close;
    QDateTime                    m_lastProximity;
};

#endif
