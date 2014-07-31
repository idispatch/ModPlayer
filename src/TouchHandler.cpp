#include <QDebug>
#include <bb/cascades/TouchEvent>
#include "TouchHandler.hpp"

template<>
int InstanceCounter<TouchHandler>::s_count;
template<>
int InstanceCounter<TouchHandler>::s_maxCount;

using namespace bb::cascades;

TouchHandler::TouchHandler(QObject *parent)
    : QObject(parent),
      m_tapRadius(20),
      m_swipeRadius(30),
      m_touched(false) {
}

TouchHandler::~TouchHandler() {
}

int TouchHandler::tapRadius() const {
    return m_tapRadius;
}

void TouchHandler::setTapRadius(int value) {
    m_tapRadius = value;
}

int TouchHandler::swipeRadius() const {
    return m_swipeRadius;
}

void TouchHandler::setSwipeRadius(int value) {
    m_swipeRadius = value;
}

void TouchHandler::reset() {
    m_localDown.reset();
    m_screenDown.reset();
    m_windowDown.reset();

    m_localMove.reset();
    m_screenMove.reset();
    m_windowMove.reset();
}

void TouchHandler::handle(TouchEvent* event) {
    if(event == NULL) {
        return;
    }
#if 0
    const char * touchType;
    switch(event->touchType()) {
    case TouchType::Up:
        touchType = "Up";
        break;
    case TouchType::Down:
        touchType = "Down";
        break;
    case TouchType::Move:
        touchType = "Move";
        break;
    case TouchType::Cancel:
        touchType = "Cancel";
        break;
    default:
        touchType = "?";
        break;
    }
    qDebug() << "Touch:" << touchType
             << "lx=" << event->localX()
             << "ly=" << event->localY()
             << "sx=" << event->screenX()
             << "sy=" << event->screenY()
             << "wx=" << event->windowX()
             << "wy=" << event->windowY();
#endif
    switch(event->touchType()) {
    case TouchType::Cancel:
    case TouchType::Up:
        if(m_touched) {
            m_touched = false;

            Point localUp;
            if(event->touchType() == TouchType::Cancel) {
                localUp = m_localMove;
            } else {
                localUp.set(event->localX(), event->localY());
            }
#if 0
            qDebug() << "TouchHandler: dx=" << localUp.adx(m_localDown) << "dy=" << localUp.ady(m_localDown);
#endif
            if(localUp.adx(m_localDown) < m_tapRadius &&
               localUp.ady(m_localDown) < m_tapRadius)
            {
#if 0
                qDebug() << "TouchHandler::tap";
#endif
                emit tap(localUp.x(), localUp.y());
            }
            else if(localUp.adx(m_localDown) > m_swipeRadius &&
                    localUp.ady(m_localDown) < m_swipeRadius)
            {
#if 0
                qDebug() << "TouchHandler::horizontalSwipe";
#endif
                emit horizontalSwipe(m_localDown.x(), m_localDown.y(), localUp.x(), localUp.y());
            }
            else if(localUp.adx(m_localDown) < m_swipeRadius &&
                    localUp.ady(m_localDown) > m_swipeRadius)
            {
#if 0
                qDebug() << "TouchHandler::verticalSwipe";
#endif
                emit verticalSwipe(m_localDown.x(), m_localDown.y(), localUp.x(), localUp.y());
            }
        }
        else
        {
            reset();
        }
        break;
    case TouchType::Down:
        m_touched = true;
        m_localDown.set(event->localX(), event->localY());
        m_screenDown.set(event->screenX(), event->screenY());
        m_windowDown.set(event->windowX(), event->windowY());

        m_localMove = m_localDown;
        m_screenMove = m_screenDown;
        m_windowMove = m_windowDown;
        break;
    case TouchType::Move:
        if(m_touched)
        {
            m_localMove.set(event->localX(), event->localY());
            m_screenMove.set(event->screenX(), event->screenY());
            m_windowMove.set(event->windowX(), event->windowY());
        }
        break;
    default:
        m_touched = false;
        reset();
        break;
    }
}
