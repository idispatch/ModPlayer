#ifndef UNPACKER_HPP_
#define UNPACKER_HPP_

#include <QObject>

class Unpacker : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString tempPath READ tempPath FINAL)
    Q_PROPERTY(QString cachePath READ cachePath FINAL)
public:
    Unpacker(QObject * parent = 0);

    QString unpackFile(QString const& compressedFile);

    QString tempPath() const;
    QString cachePath() const;
Q_SIGNALS:
private:
    Q_DISABLE_COPY(Unpacker)
};

#endif
