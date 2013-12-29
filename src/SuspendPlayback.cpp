#include "SuspendPlayback.hpp"
#include "Playback.hpp"

SuspendPlayback::SuspendPlayback(Playback * playback)
    : m_playback(playback),
      m_wasPlaying(playback->state() == Playback::Playing)
{
    try
    {
        if(m_wasPlaying)
        {
            m_playback->stop();
        }
    } catch(...) {
    }
}

SuspendPlayback::~SuspendPlayback() {
    try
    {
        if(m_wasPlaying)
        {
            m_playback->play();
        }
    } catch(...) {
    }
}
