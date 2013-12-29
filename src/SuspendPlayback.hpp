#ifndef SUSPENDPLAYBACK_HPP_
#define SUSPENDPLAYBACK_HPP_

class Playback;

class SuspendPlayback
{
private:
    Playback * m_playback;
    const bool m_wasPlaying;
private:
    SuspendPlayback(SuspendPlayback const& other);
    SuspendPlayback& operator = (SuspendPlayback const& other);
public:
    SuspendPlayback(Playback * playback);
    ~SuspendPlayback();
};

#endif
