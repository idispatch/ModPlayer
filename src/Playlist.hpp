#ifndef PLAYLIST_HPP_
#define PLAYLIST_HPP_

#include <QObject>
#include <QMetaType>
#include <QDebug>
#include <vector>
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
    Q_PROPERTY(int remainingCount READ remainingCount NOTIFY remainingCountChanged FINAL)
    Q_PROPERTY(QVariant currentSong READ currentSong NOTIFY currentSongChanged FINAL)
    Q_PROPERTY(int position READ position NOTIFY positionChanged FINAL)
    Q_PROPERTY(bool nextAvailable READ nextAvailable NOTIFY nextAvailableChanged FINAL)
    Q_PROPERTY(bool previousAvailable READ previousAvailable NOTIFY previousAvailableChanged FINAL)
    Q_PROPERTY(bool isRandom READ isRandom NOTIFY isRandomChanged FINAL)
    Q_PROPERTY(bool isCyclic READ isCyclic NOTIFY isCyclicChanged FINAL)
public:
    Playlist(QObject * parent = 0);
    Playlist(Mode mode, QObject * parent = 0);
    ~Playlist();

    Q_INVOKABLE void clear();
    Q_INVOKABLE void reset();
    Q_INVOKABLE void add(QVariant item);
    Q_INVOKABLE void assign(QVariant item);
    Q_INVOKABLE QVariant next();
    Q_INVOKABLE QVariant previous();

    Q_INVOKABLE bool isRandom() const;
    Q_INVOKABLE bool isCyclic() const;

    QVariant currentSong() const;
    int position() const;
    int count() const;
    int remainingCount() const;
    bool previousAvailable() const;
    bool nextAvailable() const;
    Mode mode() const;
    void setMode(Mode mode);

    using InstanceCounter<Playlist>::getInstanceCount;
    using InstanceCounter<Playlist>::getMaxInstanceCount;
private:
    class State {
        const Mode m_mode;
        const QVariant m_currentSong;
        const int m_position;
        const int m_count;
        const int m_remainingCount;
        const bool m_nextAvailable;
        const bool m_previousAvailable;
        const bool m_isRandom;
        const bool m_isCyclic;
        State(State const&);
        State& operator = (State const&);
    public:
        State(Playlist * p)
            : m_mode(p->mode()),
            m_currentSong(p->currentSong()),
            m_position(p->position()),
            m_count(p->count()),
            m_remainingCount(p->remainingCount()),
            m_nextAvailable(p->nextAvailable()),
            m_previousAvailable(p->previousAvailable()),
            m_isRandom(p->isRandom()),
            m_isCyclic(p->isCyclic())
        {}
        Mode mode() const { return m_mode; }
        QVariant currentSong() const { return m_currentSong; }
        int position() const { return m_position; }
        int count() const { return m_count; }
        int remainingCount() const { return m_remainingCount; }
        bool nextAvailable() const { return m_nextAvailable; }
        bool previousAvailable() const { return m_previousAvailable; }
        bool isRandom() const { return m_isRandom; }
        bool isCyclic() const { return m_isCyclic; }
    };
    void addImpl(QVariant value);
    void notify(State const& state);
Q_SIGNALS:
    void modeChanged();
    void currentSongChanged();
    void positionChanged();
    void countChanged();
    void remainingCountChanged();
    void nextAvailableChanged();
    void previousAvailableChanged();
    void isRandomChanged();
    void isCyclicChanged();
private:
    Mode                    m_mode;
    std::vector<QVariant>   m_songs;
    size_t                  m_position;
};

Q_DECLARE_METATYPE(Playlist*);

QDebug operator << (QDebug dbg, Playlist const&p);

#endif
