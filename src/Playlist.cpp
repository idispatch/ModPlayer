#include "Playlist.hpp"
#include <cstdlib>
#include <algorithm>
#include <iterator>

Playlist::Playlist() 
    : m_mode(PlaylistOnce) 
{}

Playlist::Playlist(Mode mode) 
    : m_mode(mode) 
{}

Playlist::~Playlist() 
{}

bool Playlist::isRandom() const {
    return m_mode == PlaylistRandomCycle || m_mode == PlaylistRandomOnce;
}

bool Playlist::isCyclic() const {
    return m_mode == PlaylistCycle || m_mode == PlaylistRandomCycle || m_mode == SongCycle;
}

int Playlist::current() const {
    return m_pending.empty() ? -1 : m_pending[0];
}

void Playlist::clear() {
    State state(this);
    m_history.clear();
    m_pending.clear();
    notify(state);
}

void Playlist::reset() {
    State state(this);
    std::copy(m_pending.begin(), m_pending.end(), std::back_inserter(m_history));
    m_pending.assign(m_history.begin(), m_history.end());
    m_history.clear();
    switch(m_mode) {
    case PlaylistRandomOnce:
    case PlaylistRandomCycle:
        std::random_shuffle(m_pending.begin(), m_pending.end());
        break;
    default:
        break;
    }
    notify(state);
}

void Playlist::add(int id) {
    State state(this);
    m_pending.push_back(id);
    notify(state);
}

int Playlist::count() const {
    return m_pending.size() + m_history.size();
}

int Playlist::remaining() const {
    switch(m_mode) {
    case SongOnce:
        return m_pending.empty() ? 0 : 1;
    case SongCycle:
        return m_pending.empty() && m_history.empty() ? 0 : 1;
    case PlaylistOnce:
    case PlaylistRandomOnce:
        return m_pending.size();
    case PlaylistCycle:
    case PlaylistRandomCycle:
        return m_pending.size() + m_history.size();
    default:
        return 0;
    }
}

bool Playlist::empty() const {
    return m_pending.empty();
}

int Playlist::next() {
    State state(this);
    int result = -1;
    switch(m_mode) {
    case SongOnce:
    case PlaylistOnce:
        if(!m_pending.empty()) {
            m_history.push_back(m_pending.front());
            m_pending.erase(m_pending.begin());
        }
        break;
    case SongCycle:
        break;
    case PlaylistCycle:
        if(!m_pending.empty()) {
            m_history.push_back(m_pending.front());
            m_pending.erase(m_pending.begin());
        }
        if(m_pending.empty()) {
            m_pending.assign(m_history.begin(), m_history.end());
            m_history.clear();
        }
        break;
    case PlaylistRandomOnce:
        if(!m_pending.empty()) {
            m_history.push_back(m_pending.front());
            m_pending.erase(m_pending.begin());

            if(!m_pending.empty()) {
                std::vector<int>::iterator i = m_pending.begin();
                std::advance(i, std::rand() % m_pending.size());
                std::iter_swap(m_pending.begin(), i);
            }
        }
        break;
    case PlaylistRandomCycle:
        if(m_pending.empty()) {
            m_pending.assign(m_history.begin(), m_history.end());
            m_history.clear();
            std::random_shuffle(m_pending.begin(), m_pending.end());
        } else {
            m_history.push_back(m_pending.front());
            m_pending.erase(m_pending.begin());

            if(m_pending.empty()) {
                m_pending.assign(m_history.begin(), m_history.end());
                m_history.clear();
                std::random_shuffle(m_pending.begin(), m_pending.end());
            } else {
                std::vector<int>::iterator i = m_pending.begin();
                std::advance(i, std::rand() % m_pending.size());
                std::iter_swap(m_pending.begin(), i);
            }
        }
        break;
    default:
        break;
    }

    result = current();

    notify(state);
    return result;
}

int Playlist::previous() {
    State state(this);
    int result = -1;
    //TODO:
    notify(state);
    return result;
}

bool Playlist::atEnd() const {
    switch(m_mode) {
    case SongOnce:
        return true;
    case PlaylistOnce:
    case PlaylistRandomOnce:
        return m_pending.size() <= 1;
    case SongCycle:
    case PlaylistCycle:
    case PlaylistRandomCycle:
        return (m_pending.size() + m_history.size()) == 0;
    default:
        return true;
    }
}

bool Playlist::previousAvailable() const {
    switch(m_mode) {
    case SongOnce:
    case PlaylistOnce:
    case PlaylistRandomOnce:
        return !m_history.empty();
    case SongCycle:
    case PlaylistCycle:
    case PlaylistRandomCycle:
        return (m_pending.size() + m_history.size()) > 0;
    default:
        return false;
    }
}

bool Playlist::nextAvailable() const {
    return !atEnd();
}

Mode Playlist::mode() const {
    return m_mode;
}

void Playlist::setMode(Mode mode) {
    switch(mode) {
    case SongOnce:
    case PlaylistOnce:
    case PlaylistRandomOnce:
    case SongCycle:
    case PlaylistCycle:
    case PlaylistRandomCycle:
        break;
    default:
        mode = m_mode;
        break;
    }
    if(mode != m_mode) {
        State state(this);
        notify(state);
        emit modeChanged();
    }
}

void Playlist::notify(State const& state) {
    if(state.current() != current()) {
        emit currentChanged();
    }
    if(state.count() != count()) {
        emit countChanged();
    }
    if(state.remaining() != remaining()) {
        emit remainingChanged();
    }
    if(state.empty() != empty()) {
        emit emptyChanged();
    }
    if(state.atEnd() != atEnd()) {
        emit atEndChanged();
    }
    if(state.nextAvailable() != nextAvailable()) {
        emit nextAvailableChanged();
    }
    if(state.previousAvailable() != previousAvailable()) {
        emit previousAvailableChanged();
    }
}

#if 0
void dump(Playlist const& p) {
    std::cout << "CurrentID: " << p.current() << std::endl;
    std::cout << "Count: " << p.count() << std::endl;
    std::cout << "Remaining: " << p.remaining() << std::endl;
    std::cout << "Empty: " << (p.empty() ? "true":"false") << std::endl;
    std::cout << "AtEnd: " << (p.atEnd() ? "true":"false") << std::endl;
    std::cout << "PreviousAvailble: " << (p.previousAvailable() ? "true":"false") << std::endl;
    std::cout << "NextAvailable: " << (p.nextAvailable() ? "true":"false") << std::endl;
}
#endif
