#ifndef CACHEFILEINFO_HPP
#define CACHEFILEINFO_HPP

#include <QDebug>
#include <QString>
#include <sys/stat.h>

class CacheFileInfo
{
    qint64  m_size;
    time_t  m_time;
    QString m_name;
public:
    CacheFileInfo()
        : m_size(0),
          m_time(0)
    {}

    CacheFileInfo(qint64  size,
                  time_t  time,
                  QString name)
        : m_size(size),
          m_time(time),
          m_name(name)
    {}

    CacheFileInfo(CacheFileInfo const& other)
        : m_size(other.size()),
          m_time(other.time()),
          m_name(other.name())
    {}

    CacheFileInfo& operator = (CacheFileInfo const& other) {
        if(this!=&other)
        {
            m_size = other.size();
            m_time = other.time();
            m_name = other.name();
        }
        return *this;
    }

    qint64 size() const {
        return m_size;
    }

    time_t time() const {
        return m_time;
    }

    QString const& name() const {
        return m_name;
    }
};

inline
bool operator == (CacheFileInfo const& first, CacheFileInfo const& second) {
    return first.time() == second.time() &&
           first.size() == second.size() &&
           first.name() == second.name();
}

inline
bool operator != (CacheFileInfo const& first, CacheFileInfo const& second) {
    return first.time() != second.time() ||
           first.size() != second.size() ||
           first.name() != second.name();
}

inline
bool operator < (CacheFileInfo const& first, CacheFileInfo const& second) {
    if(first.time() < second.time()) {
        return true;
    } else if(first.time() == second.time()) {
        if(first.size() < second.size()) {
            return true;
        } else if(first.size() == second.size()) {
            if(first.name() < second.name()) {
                return true;
            }
        }
    }
    return false;
}

inline
QDebug operator << (QDebug stream, CacheFileInfo const& value) {
    stream.nospace() << "[" << " name="  << value.name()
                     << ", time=" << value.time()
                     << ", size=" << value.size()
                     << "]";
    return stream.space();
}

#endif
