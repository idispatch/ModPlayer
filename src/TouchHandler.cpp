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
      m_tapRadius(50),
      m_touched(false),
      m_localX(0),
      m_localY(0) {
}

TouchHandler::~TouchHandler() {
}

int TouchHandler::tapRadius() const {
    return m_tapRadius;
}

void TouchHandler::setTapRadius(int value) {
    m_tapRadius = value;
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
    case TouchType::Up:
        if(m_touched) {
            m_touched = false;
            int x = event->localX();
            int y = event->localY();
            if(abs(x - m_localX) < m_tapRadius &&
               abs(y - m_localY) < m_tapRadius) {
                emit tap(x, y);
            }
        }
        break;
    case TouchType::Down:
        m_touched = true;
        m_localX = event->localX();
        m_localY = event->localY();
        break;
    case TouchType::Move:
        break;
    case TouchType::Cancel:
    default:
        m_touched = false;
        m_localX = 0;
        m_localY = 0;
        break;
    }
}
