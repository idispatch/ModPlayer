#include "FileUtils.hpp"
#include <QFile>

bool FileUtils::isAbsolute(QString const& fileName) {
    return fileName.startsWith('/');
}

bool FileUtils::isRelative(QString const& fileName) {
    return !isAbsolute(fileName);
}

QString FileUtils::fileNameOnly(QString const& fileName) {
    int index = fileName.lastIndexOf('/');
    if(index < 0) {
        return fileName;
    } else {
        return fileName.mid(index + 1);
    }
}

QString FileUtils::fileNameWithoutExtension(QString const& fileName) {
    QString name = fileNameOnly(fileName);
    int index = fileName.lastIndexOf('.');
    if(index < 0) {
        return name;
    } else {
        return fileName.mid(0, index);
    }
}

QString FileUtils::extension(QString const& fileName) {
    int index = fileName.lastIndexOf('.');
    if(index < 0) {
        return "";
    } else {
        return fileName.mid(index);
    }
}

QString FileUtils::directoryOnly(QString const& fileName) {
    int index = fileName.lastIndexOf('/');
    if(index < 0) {
        return fileName;
    } else {
        return fileName.left(index);
    }
}

QString FileUtils::joinPath(QString const& directory,
                            QString const& fileName) {
    if(isAbsolute(fileName)) {
        return fileName; // already absolute
    }

    QString result = directory;
    if(directory.endsWith('/')) {
        result += fileName;
    } else {
        result = result + "/" + fileName;
    }

    return result;
}

bool FileUtils::adjustPermissions(QString const& fileName) {
	return QFile::setPermissions(fileName,
                                 QFile::ReadOwner | QFile::WriteOwner |
                                 QFile::ReadUser | QFile::WriteUser |
                                 QFile::ReadGroup | QFile::WriteGroup |
                                 QFile::ReadOther | QFile::WriteOther);
}
