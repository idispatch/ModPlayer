#include "Unpacker.hpp"
#include "FileUtils.hpp"
#include "zip/JlCompress.h"
#include <QDebug>
#include <QDir>
#include <QStringList>

#ifdef _DEBUG
//#define VERBOSE_LOGGING
#else
#endif

template<>
int InstanceCounter<Unpacker>::s_count;
template<>
int InstanceCounter<Unpacker>::s_maxCount;

Unpacker::Unpacker(QObject * parent)
    : QObject(parent) {
}

Unpacker::~Unpacker() {
#ifdef VERBOSE_LOGGING
    qDebug() << "Unpacker::~Unpacker()";
#endif
}

QString Unpacker::tempPath() const {
    return QDir::tempPath();
}
QString Unpacker::cachePath() const {
    return QDir::homePath();
}

QString Unpacker::unpackFile(QString const& compressedFile) {
    QString newFile;
    if(compressedFile.isEmpty()) {
        return "";
    }
    if(!compressedFile.toLower().endsWith(".zip")) {
        qDebug() << "Not a Zip file:" << compressedFile;
        newFile = FileUtils::joinPath(cachePath(), FileUtils::fileNameOnly(compressedFile.toLower()));
        if(FileUtils::exists(newFile)) {
            qDebug() << "File" << newFile << "exists, deleting";
            if(!QFile::remove(newFile)) {
                qDebug() << "Failed to delete file" << newFile;
                newFile = "";
            } else {
                qDebug() << "File" << newFile << "deleted";
            }
        }

        if(QFile::copy(compressedFile, newFile)) {
            if(!FileUtils::adjustPermissions(newFile)) {
                qDebug() << "Failed to set permissions for file " << newFile;
                newFile = "";
            }
        } else {
            qDebug() << "Failed to copy file" << compressedFile << "to" << newFile;
            newFile = "";
        }
    } else {
        do {
            qDebug() << "Contents of" << compressedFile;
            QStringList list = JlCompress::getFileList(compressedFile);
            qDebug() << list;

            if(list.size() == 1) {
                QString newFileName = list.first();
                QString newFileNameLowered = newFileName.toLower();
                QString extractPath = FileUtils::joinPath(cachePath(), newFileNameLowered);

                if(FileUtils::exists(extractPath)) {
                    qDebug() << "File" << extractPath << "exists, deleting";
                    if(!QFile::remove(extractPath)) {
                        qDebug() << "Failed to delete file" << extractPath;
                        break;
                    } else {
                        qDebug() << "File" << extractPath << "deleted";
                    }
                }

                newFile = JlCompress::extractFile(compressedFile,
                                                  newFileName,
                                                  extractPath);
                if(newFile.isEmpty()) {
                    qDebug() << "Failed to extract" << extractPath;
                } else {
                    qDebug() << "Successfully extracted" << newFile;
                    if(!FileUtils::adjustPermissions(newFile)) {
                        qDebug() << "Failed to set permissions for file " << newFile;
                    }
                }
            } else {
                qDebug() << "Expects one file in Zip, found" << list;
            }
        }
        while(false);
    }
    return newFile;
}

