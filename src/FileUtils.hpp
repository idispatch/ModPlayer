#ifndef FILEUTILS_HPP_
#define FILEUTILS_HPP_

#include <QString>

class FileUtils {
public:
    static bool isAbsolute(QString const& fileName);
    static bool isRelative(QString const& fileName);
    static QString joinPath(QString const& directory,
                            QString const& fileName);
    static QString fileNameOnly(QString const& fileName);
    static QString fileNameWithoutExtension(QString const& fileName);
    static QString directoryOnly(QString const& fileName);
    static QString extension(QString const& fileName);

    static bool adjustPermissions(QString const& fileName);
};

#endif /* FILEUTILS_HPP_ */

