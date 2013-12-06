#ifndef SUSPENDPLAYBACK_HPP_
#define SUSPENDPLAYBACK_HPP_

class ModPlayback;

class SuspendPlayback
{
private:
    ModPlayback * m_playback;
    const bool m_wasPlaying;
private:
    SuspendPlayback(SuspendPlayback const& other);
    SuspendPlayback& operator = (SuspendPlayback const& other);
public:
    SuspendPlayback(ModPlayback * playback);
    ~SuspendPlayback();
};

#endif
