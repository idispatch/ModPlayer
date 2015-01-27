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
        emit sleepRemainingChanged();
    }
}

int SleepTimer::sleepRemaining() const {
    int minutesRemaining;
    if(timerActive()) {
        minutesRemaining = totalSecondsRemaining() / 60;
    } else {
        minutesRemaining = sleepTimeout();
    }
    return minutesRemaining;
}

int SleepTimer::totalSecondsRemaining() const {
    int deltaMilliseconds = m_sleepTimer.interval() - m_sleepTimerStarted.elapsed();
    if(deltaMilliseconds < 0) {
        deltaMilliseconds = 0;
    }
    const int totalSeconds = deltaMilliseconds / 1000;
    return totalSeconds;
}

bool SleepTimer::timerActive() const {
    return m_sleepTimer.isActive();
}

QString SleepTimer::status() const {
    const int totalSeconds = totalSecondsRemaining();
    const int minutes = totalSeconds / 60;
    const int seconds = totalSeconds % 60;
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
        emit sleepRemainingChanged();
    }
}

void SleepTimer::onTimerExpired() {
    emit sleepRemainingChanged();
    emit timerActiveChanged();
    emit statusChanged();
    m_sleepTimerRemainingUpdate.stop();
    emit expired();
}

void SleepTimer::onUpdateStatus() {
    emit statusChanged();
    emit sleepRemainingChanged();
}
