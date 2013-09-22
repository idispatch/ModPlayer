#ifndef FILEUTILS_HPP_
#define FILEUTILS_HPP_

#include <QString>

class FileUtils {
public:
    static QString joinPath(QString const& directory,
                            QString const& fileName);
    static QString fileNameOnly(QString const& fileName);
};

#endif /* FILEUTILS_HPP_ */
