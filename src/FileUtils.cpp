#include "FileUtils.hpp"

bool FileUtils::isAbsolute(QString const& fileName) {
    return fileName.startsWith('/');
}

QString FileUtils::fileNameOnly(QString const& fileName) {
    int index = fileName.lastIndexOf('/');
    if(index < 0)
    {
        return fileName;
    }
    else
    {
        return fileName.mid(index + 1);
    }
}

QString FileUtils::joinPath(QString const& directory,
                            QString const& fileName) {
    if(isAbsolute(fileName))
    {
        return fileName; // already absolute
    }

    QString result = directory;
    if(directory.endsWith('/'))
    {
        result += fileName;
    }
    else
    {
        result = result + "/" + fileName;
    }

    return result;
}

