#ifndef PLAYLIST_HPP_
#define PLAYLIST_HPP_

#include <QObject>
#include <QMetaType>
#include <QDebug>
#include <vector>
#include <deque>
#include "InstanceCounter.hpp"

class Playlist : public QObject,
                 public InstanceCounter<Playlist> {
    Q_OBJECT
    Q_DISABLE_COPY(Playlist)
public:
    enum Mode {
        // 1st bit: 0=song,       1=playlist
        // 2nd bit: 0=once,       1=cycle
        // 3rd bit: 0=sequential, 1=random
        SongOnce            = 0x000, //Play single song once
        SongCycle           = 0x010, //Play single song in cycle
        PlaylistOnce        = 0x001, //Play playlist sequentially once and then stop
        PlaylistCycle       = 0x011, //Play playlist sequentially and repeat
        PlaylistRandomOnce  = 0x101, //Play playlist randomly and then stop
        PlaylistRandomCycle = 0x111  //Play playlist randomly and repeat
    };
    Q_ENUMS(Mode)
private:
    Q_PROPERTY(Mode mode READ mode WRITE setMode NOTIFY modeChanged FINAL)
    Q_PROPERTY(int count READ count NOTIFY countChanged FINAL)
    Q_PROPERTY(int remaining READ remaining NOTIFY remainingChanged FINAL)
    Q_PROPERTY(int current READ current NOTIFY currentChanged FINAL)
    Q_PROPERTY(bool empty READ empty NOTIFY emptyChanged FINAL)
    Q_PROPERTY(bool atEnd READ atEnd NOTIFY atEndChanged FINAL)
    Q_PROPERTY(bool nextAvailable READ nextAvailable NOTIFY nextAvailableChanged FINAL)
    Q_PROPERTY(bool previousAvailable READ previousAvailable NOTIFY previousAvailableChanged FINAL)
public:
    Playlist(QObject * parent = 0);
    Playlist(Mode mode, QObject * parent = 0);
    ~Playlist();

    Q_INVOKABLE void clear();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void add(int id);
    Q_INVOKABLE int next();
    Q_INVOKABLE int previous();

    bool isRandom() const;
    bool isCyclic() const;

    int current() const;
    int count() const;
    int remaining() const;
    bool empty() const;
    bool atEnd() const;
    bool previousAvailable() const;
    bool nextAvailable() const;
    Mode mode() const;
    void setMode(Mode mode);

    using InstanceCounter<Playlist>::getInstanceCount;
    using InstanceCounter<Playlist>::getMaxInstanceCount;
private:
    class State {
        const Mode m_mode;
        const int m_current;
        const int m_count;
        const int m_remaining;
        const bool m_empty;
        const bool m_atEnd;
        const bool m_nextAvailable;
        const bool m_previousAvailable;
        State(State const&);
        State& operator = (State const&);
    public:
        State(Playlist * p)
            : m_mode(p->mode()),
            m_current(p->current()),
            m_count(p->count()),
            m_remaining(p->remaining()),
            m_empty(p->empty()),
            m_atEnd(p->atEnd()),
            m_nextAvailable(p->nextAvailable()),
            m_previousAvailable(p->previousAvailable())
        {}
        Mode mode() const { return m_mode; }
        int current() const { return m_current; }
        int count() const { return m_count; }
        int remaining() const { return m_remaining; }
        bool empty() const { return m_empty; }
        bool atEnd() const { return m_atEnd; }
        bool nextAvailable() const { return m_nextAvailable; }
        bool previousAvailable() const { return m_previousAvailable; }
    };
    void notify(State const& state);
Q_SIGNALS:
    void modeChanged();
    void currentChanged();
    void countChanged();
    void remainingChanged();
    void emptyChanged();
    void atEndChanged();
    void nextAvailableChanged();
    void previousAvailableChanged();
private:
    Mode m_mode;
    std::vector<int> m_pending;
    std::deque<int> m_history;
};

Q_DECLARE_METATYPE(Playlist*);

QDebug operator << (QDebug dbg, Playlist const &p);

#endif
