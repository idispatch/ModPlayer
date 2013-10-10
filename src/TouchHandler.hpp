#ifndef TOUCHHANDLER_HPP_
#define TOUCHHANDLER_HPP_

#include <QObject>
#include <QMetaType>
#include "InstanceCounter.hpp"

namespace bb {
    namespace cascades {
        class TouchEvent;
    }
}

class TouchHandler : public QObject,
                     public InstanceCounter<TouchHandler> {
    Q_OBJECT
    Q_PROPERTY(int tapRadius READ tapRadius WRITE setTapRadius FINAL)
public:
    TouchHandler(QObject *parent);
    ~TouchHandler();

    int tapRadius() const;
    void setTapRadius(int);

    using InstanceCounter<TouchHandler>::getInstanceCount;
    using InstanceCounter<TouchHandler>::getMaxInstanceCount;
Q_SIGNALS:
    void tap(int x, int y);
public Q_SLOTS:
    void handle(bb::cascades::TouchEvent* event);
private:
    Q_DISABLE_COPY(TouchHandler)
private:
    int m_tapRadius;
    bool m_touched;
    int m_localX;
    int m_localY;
};

Q_DECLARE_METATYPE(TouchHandler*);

#endif /* TOUCHHANDLER_HPP_ */
