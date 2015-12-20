#ifndef FILESYSTEM_HPP_
#define FILESYSTEM_HPP_

#include <QObject>
#include <QMetaType>
#include <QString>
#include <QStringList>
#include <bb/cascades/ArrayDataModel>

class FileSystem : public QObject {
    Q_OBJECT
    Q_DISABLE_COPY(FileSystem)
    QStringList m_filters;
public:
    FileSystem(const QStringList& filters, QObject * parent = 0);

    Q_INVOKABLE bb::cascades::ArrayDataModel* listFiles(const QString& path, QObject * parent = 0);
private:
    bb::cascades::ArrayDataModel* listRoot(QObject * parent = 0);
    bool fileMatches(const QString& fileName) const;
};

Q_DECLARE_METATYPE(FileSystem*);

#endif
