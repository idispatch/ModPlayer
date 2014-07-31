#include "FileUtils.hpp"
#include <dirent.h>
#include <errno.h>
#include <QFile>
#include <QDebug>

#ifdef _DEBUG
//#define VERBOSE_LOGGING
#else
#endif

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

QString FileUtils::fileNameOnlyWithoutExtension(QString const& fileName) {
    QString name = fileNameOnly(fileName);
    int index = name.lastIndexOf('.');
    if(index < 0) {
        return name;
    } else {
        return name.mid(0, index);
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

bool FileUtils::exists(QString const& fileName) {
    struct stat64 st;
    int rc = ::stat64(fileName.toUtf8().constData(), &st);
#ifdef VERBOSE_LOGGING
    if(rc != 0) {
        qDebug() << "File" << fileName << "does not exist";
    }
#endif
    return rc == 0;
}
