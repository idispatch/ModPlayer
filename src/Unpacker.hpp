#ifndef UNPACKER_HPP_
#define UNPACKER_HPP_

#include <QObject>

class Unpacker : public QObject {
    Q_OBJECT
public:
    Unpacker(QObject * parent = 0);

    QString unpackFile(QString const& compressedFilePath);
Q_SIGNALS:
private:
    Q_DISABLE_COPY(Unpacker)
};

#endif
