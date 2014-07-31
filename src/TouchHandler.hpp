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
    Q_PROPERTY(int swipeRadius READ swipeRadius WRITE setSwipeRadius FINAL)
public:
    TouchHandler(QObject *parent);
    ~TouchHandler();

    int tapRadius() const;
    void setTapRadius(int);

    int swipeRadius() const;
    void setSwipeRadius(int);

    using InstanceCounter<TouchHandler>::getInstanceCount;
    using InstanceCounter<TouchHandler>::getMaxInstanceCount;
Q_SIGNALS:
    void tap(int x, int y);
    void horizontalSwipe(int x0, int y0, int x1, int y1);
    void verticalSwipe(int x0, int y0, int x1, int y1);
public Q_SLOTS:
    void handle(bb::cascades::TouchEvent* event);
private:
    Q_DISABLE_COPY(TouchHandler)
    void reset();
private:

    int m_tapRadius;
    int m_swipeRadius;
    bool m_touched;

    class Point
    {
        int m_x;
        int m_y;
    public:
        Point() {
            reset();
        }
        int x() const { return m_x; }
        int y() const { return m_y; }

        int dx(Point const& other) const {
            return m_x - other.m_x;
        }
        int dy(Point const& other) const {
            return m_y - other.m_y;
        }

        int adx(Point const& other) const {
            return std::abs(dx(other));
        }
        int ady(Point const& other) const {
            return std::abs(dy(other));
        }

        void set(int x, int y) {
            m_x = x;
            m_y = y;
        }
        void reset() {
            set(0, 0);
        }
    };

    Point m_localDown;
    Point m_screenDown;
    Point m_windowDown;

    Point m_localMove;
    Point m_screenMove;
    Point m_windowMove;
};

Q_DECLARE_METATYPE(TouchHandler*);

#endif /* TOUCHHANDLER_HPP_ */
