#include "FileSystem.hpp"
#include "FileEntry.hpp"
#include "FileUtils.hpp"

FileSystem::FileSystem(QStringList const& filters, QObject * parent)
    : QObject(parent),
      m_filters(filters) {
}

bool FileSystem::fileMatches(QString const& fileName) {
    QString const& extension = FileUtils::extension(fileName);
    return m_filters.contains(extension, Qt::CaseInsensitive);
}

bb::cascades::ArrayDataModel* FileSystem::listRoot() {
    static const char * locations[] = {
        "/accounts/1000/shared/music",
        "/accounts/1000/shared/documents",
        "/accounts/1000/shared/downloads",
        "/accounts/1000/removable/sdcard",
        "/accounts/1000/shared/Box",
        "/accounts/1000/shared/Dropbox"
    };

    QList<FileEntry*> roots;
    for(size_t i = 0; i < sizeof(locations)/sizeof(locations[0]); ++i) {
        struct stat64 st;
        QString absoluteFileName(locations[i]);
        if(0 == ::stat64(absoluteFileName.toUtf8().constData(), &st)) {
            FileEntry * fileEntry = new FileEntry(absoluteFileName, st);
            roots << fileEntry;
        }
    }

    bb::cascades::ArrayDataModel * model = new bb::cascades::ArrayDataModel();
    foreach(FileEntry * root, roots) {
        model->append(QVariant::fromValue(static_cast<QObject*>(root)));
    }
    return model;
}

bb::cascades::ArrayDataModel* FileSystem::listFiles(QString const& path) {
    if(path.isEmpty() || path == "/") {
        return listRoot();
    }

    bb::cascades::ArrayDataModel * model = new bb::cascades::ArrayDataModel();

    QList<FileEntry*> foundFiles;
    QList<FileEntry*> foundDirectories;

    DIR *dirp;
    if ((dirp = ::opendir(path.toUtf8().constData())) != NULL) {
        struct dirent64 *direntItem;
        do {
            if ((direntItem = ::readdir64(dirp)) != NULL) {
                if(direntItem->d_name[0] == '.' &&
                   (direntItem->d_name[1] == '\0' ||
                    (direntItem->d_name[1] == '.' && direntItem->d_name[2] == '\0'))) {
                    continue;
                }

                QString fileName = QString::fromUtf8(direntItem->d_name);
                QString absoluteFileName = FileUtils::joinPath(path,
                                                               fileName);
                struct stat64 st;
                if(0 == ::stat64(absoluteFileName.toUtf8().constData(), &st)) {
                    if(st.st_mode & S_IFREG) {
                        if(fileMatches(fileName)) {
                            FileEntry * fileEntry = new FileEntry(absoluteFileName, st, model);
                            foundFiles << fileEntry;
                        }
                    } else if(st.st_mode & S_IFDIR) {
                        FileEntry * fileEntry = new FileEntry(absoluteFileName, st, model);
                        foundDirectories << fileEntry;
                    }
                }
            }
        } while (direntItem != NULL);
        ::closedir(dirp);
    }

    foreach(FileEntry * directory, foundDirectories) {
        model->append(QVariant::fromValue(static_cast<QObject*>(directory)));
    }
    foreach(FileEntry * file, foundFiles) {
        model->append(QVariant::fromValue(static_cast<QObject*>(file)));
    }

    return model;
}
