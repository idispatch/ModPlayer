#ifndef SLEEPTIMER_HPP
#define SLEEPTIMER_HPP

#include <QObject>
#include <QMetaType>
#include <QTime>
#include <QTimer>
#include <QString>

class SleepTimer : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(SleepTimer)

    Q_PROPERTY(int sleepTimeout READ sleepTimeout WRITE setSleepTimeout NOTIFY timeoutChanged FINAL)
    Q_PROPERTY(int sleepRemaining READ sleepRemaining NOTIFY sleepRemainingChanged FINAL)
    Q_PROPERTY(bool timerActive READ timerActive NOTIFY timerActiveChanged FINAL)
    Q_PROPERTY(QString status READ status NOTIFY statusChanged FINAL)
public:
    SleepTimer(QObject * parent = 0);

    int sleepTimeout() const;
    void setSleepTimeout(int value);

    int sleepRemaining() const;

    bool timerActive() const;
    QString status() const;

    Q_INVOKABLE void start();
    Q_INVOKABLE void cancel();

Q_SIGNALS:
    void expired();
    void timeoutChanged();
    void timerActiveChanged();
    void statusChanged();
    void sleepRemainingChanged();
private slots:
    void onTimerExpired();
    void onUpdateStatus();
private:
    int totalSecondsRemaining() const;
private:
    QTimer  m_sleepTimer;
    QTimer  m_sleepTimerRemainingUpdate;
    QTime   m_sleepTimerStarted;
};

Q_DECLARE_METATYPE(SleepTimer*);

#endif
