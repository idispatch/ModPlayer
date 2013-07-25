#include "Unpacker.hpp"
#include "zip/JlCompress.h"
#include <QDebug>
#include <QDir>
#include <QStringList>

Unpacker::Unpacker(QObject * parent)
    : QObject(parent) {
}

QString Unpacker::unpackFile(QString const& compressedFilePath) {
    if(!compressedFilePath.endsWith(".zip")) {
        qDebug() << "Not a Zip file:" << compressedFilePath;
        return "";
    } else {
        qDebug() << "Contents of" << compressedFilePath;
        QStringList list = JlCompress::getFileList(compressedFilePath);
        qDebug() << list;
        if(list.size() == 1) {
            QString extractPath = QDir::homePath() + "/" + list[0];
            QString newFile = JlCompress::extractFile(compressedFilePath, list[0], extractPath);
            qDebug() << "Extracted" << newFile;
            return newFile;
        } else {
            qDebug() << "Expects 1 file, found" << list.size();
            return "";
        }
    }
}

