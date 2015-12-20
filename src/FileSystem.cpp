#include "FileSystem.hpp"
#include "FileEntry.hpp"
#include "FileUtils.hpp"
#include <unistd.h>

namespace {
    struct FileNameLess {
        bool operator()(const FileEntry* a, const FileEntry* b) const {
            return a->fileName().compare(b->fileName()) < 0;
        }
    };
}

FileSystem::FileSystem(const QStringList& filters, QObject *parent)
: QObject(parent),
  m_filters(filters)
{}

bool FileSystem::fileMatches(const QString& fileName) const {
    const QString& extension = FileUtils::extension(fileName);
    return m_filters.contains(extension, Qt::CaseInsensitive);
}

bb::cascades::ArrayDataModel* FileSystem::listRoot(QObject *parent) {
    static const char * locations[] = {
        "/accounts/1000/shared/music",
        "/accounts/1000/shared/documents",
        "/accounts/1000/shared/downloads",
        "/accounts/1000/removable/sdcard",
        "/accounts/1000/shared/Box",
        "/accounts/1000/shared/Dropbox",
        "/accounts/1000/shared/OneDrive"
    };

    bb::cascades::ArrayDataModel * model = new bb::cascades::ArrayDataModel(parent);

    for(size_t i = 0; i < sizeof(locations)/sizeof(locations[0]); ++i) {
        // Note, not using stat64 here
        if(0 == ::access(locations[i], F_OK)) {
            struct stat64 st;
            ::memset(&st, 0, sizeof(struct stat64));
            st.st_mode = S_IFDIR; // file system access performance improvement

            FileEntry *fileEntry = new FileEntry(locations[i], st, parent);
            QObject   *obj       = static_cast<QObject*>(fileEntry);

            model->append(QVariant::fromValue(obj));
        }
    }

    return model;
}

bb::cascades::ArrayDataModel* FileSystem::listFiles(const QString& path, QObject *parent) {
    if(path.isEmpty() || path == "/") {
        return listRoot();
    }

    bb::cascades::ArrayDataModel * model = new bb::cascades::ArrayDataModel(parent);

    QList<FileEntry*> foundFiles;
    QList<FileEntry*> foundDirectories;

    DIR *dirp;
    if ((dirp = ::opendir(path.toUtf8().constData())) != NULL) {
        struct dirent64 *direntItem;
        do {
            if ((direntItem = ::readdir64(dirp)) != NULL) {
                if(direntItem->d_name[0] == '.') {
                    continue;
                }

                QString fileName = QString::fromUtf8(direntItem->d_name);
                QString absoluteFileName = FileUtils::joinPath(path, fileName);
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

    ::qSort(foundDirectories.begin(), foundDirectories.end(), FileNameLess());
    ::qSort(foundFiles.begin(), foundFiles.end(), FileNameLess());

    foreach(FileEntry * directory, foundDirectories) {
        model->append(QVariant::fromValue(static_cast<QObject*>(directory)));
    }
    foreach(FileEntry * file, foundFiles) {
        model->append(QVariant::fromValue(static_cast<QObject*>(file)));
    }

    return model;
}
