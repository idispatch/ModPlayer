#ifndef MP3EXPORT_HPP_
#define MP3EXPORT_HPP_

#include <QString>
#include "PlaybackConfig.hpp"

class Mp3Export {
public:
    static bool convert(PlaybackConfig &config,
                        QString const& inputFileName,
                        QString const& outputFileName);
};

#endif
