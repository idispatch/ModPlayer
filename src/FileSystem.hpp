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
public:
    FileSystem(QStringList const& filters, QObject * parent = 0);

    Q_INVOKABLE bb::cascades::ArrayDataModel* listFiles(QString const& path);
private:
    bb::cascades::ArrayDataModel* listRoot();
    bool fileMatches(QString const& fileName);
private:
    static QString createExtensionFilter(QString const& p);
private:
    QStringList m_filters;
};

Q_DECLARE_METATYPE(FileSystem*);

#endif
