#include "SleepTimer.hpp"

SleepTimer::SleepTimer(QObject * parent)
    : QObject(parent) {
    m_sleepTimer.setSingleShot(true);
    m_sleepTimerRemainingUpdate.setSingleShot(false);

    setSleepTimeout(15); // 15 minutes
    m_sleepTimerRemainingUpdate.setInterval(200); // 200 msec

    bool rc;
    rc = QObject::connect(&m_sleepTimer, SIGNAL(timeout()),
                          this,          SLOT(onTimerExpired()));
    Q_ASSERT(rc);

    rc = QObject::connect(&m_sleepTimerRemainingUpdate, SIGNAL(timeout()),
                          this,                         SLOT(onUpdateStatus()));
    Q_ASSERT(rc);

    Q_UNUSED(rc);
}

int SleepTimer::sleepTimeout() const {
    return m_sleepTimer.interval() / 60000;
}

void SleepTimer::setSleepTimeout(int value) {
    if(m_sleepTimer.interval() / 60000 != value) {
        m_sleepTimer.setInterval(value * 60000);
        emit timeoutChanged();
    }
}

bool SleepTimer::timerActive() const {
    return m_sleepTimer.isActive();
}

QString SleepTimer::status() const {
    int delta = m_sleepTimer.interval() - m_sleepTimerStarted.elapsed();
    if(delta < 0) {
        delta = 0;
    }
    delta /= 1000;
    const int minutes = delta / 60;
    const int seconds = delta % 60;
    return QString("%1:%2").arg(minutes, 2, 10, QChar('0')).arg(seconds, 2, 10, QChar('0'));
}

void SleepTimer::start() {
    if(!timerActive()) {
        m_sleepTimer.start();
        m_sleepTimerStarted.start();
        m_sleepTimerRemainingUpdate.start();
        emit timerActiveChanged();
        emit statusChanged();
    }
}

void SleepTimer::cancel() {
    if(timerActive()) {
        m_sleepTimer.stop();
        m_sleepTimerRemainingUpdate.stop();
        emit timerActiveChanged();
        emit statusChanged();
    }
}

void SleepTimer::onTimerExpired() {
    emit timerActiveChanged();
    emit statusChanged();
    m_sleepTimerRemainingUpdate.stop();
    emit expired();
}

void SleepTimer::onUpdateStatus() {
    emit statusChanged();
}
