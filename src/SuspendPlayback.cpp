#include "SuspendPlayback.hpp"
#include "ModPlayback.hpp"

SuspendPlayback::SuspendPlayback(ModPlayback * playback)
    : m_playback(playback),
      m_wasPlaying(playback->state() == ModPlayback::Playing)
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
