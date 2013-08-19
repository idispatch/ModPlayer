#ifndef UNPACKER_HPP_
#define UNPACKER_HPP_

#include <QObject>
#include "InstanceCounter.hpp"

class Unpacker : public QObject,
                 public InstanceCounter<Unpacker> {
    Q_OBJECT

    Q_PROPERTY(QString tempPath READ tempPath FINAL)
    Q_PROPERTY(QString cachePath READ cachePath FINAL)
public:
    Unpacker(QObject * parent = 0);

    QString unpackFile(QString const& compressedFile);

    QString tempPath() const;
    QString cachePath() const;

    using InstanceCounter<Unpacker>::getInstanceCount;
    using InstanceCounter<Unpacker>::getMaxInstanceCount;
Q_SIGNALS:
private:
    Q_DISABLE_COPY(Unpacker)
};

#endif
