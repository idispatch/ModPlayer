#include "modplug/modplug.h"
#include "Importer.hpp"
#include "FileUtils.hpp"
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>
#include <QDebug>
#include <QStack>
#include <QByteArray>
#include <bb/system/SystemProgressDialog>

#define DETAILED_LOG

using namespace bb::system;

Importer::Importer(QStringList const& filters,
                   QObject * parent)
    : QObject(parent),
      m_progress(NULL),
      m_filters(filters) {
}

Importer::~Importer() {
    destroyProgressUI();
}

void Importer::clean() {

}

void Importer::import()
{
    createProgressUI();

    updateProgressUI(tr("Importing from device memory..."), INDEFINITE);
    scanDirectory(QDir("/accounts/1000/shared"));

    updateProgressUI(tr("Importing from SD card..."), INDEFINITE);
    scanDirectory(QDir("/accounts/1000/removable/sdcard"));

    if(m_result.empty())
    {
        updateProgressUI(tr("No tracker music found"), 100);
    }
    else
    {
        int totalImported = 0;
        for (int i = 0; i < m_result.size(); i++) {
            QString const& absoluteFileName = m_result[i];
            totalImported += importFile(absoluteFileName, i * 100 / m_result.size()) ? 1 : 0;
        }

        updateProgressUI(QString(tr("Imported %1 songs")).arg(totalImported), 100);
    }

    destroyProgressUI();
}

bool Importer::importFile(QString const& fileName, int progress)
{
    QString fileNameOnly = FileUtils::fileNameOnly(fileName);
    updateProgressUI(QString(tr("Importing %1")).arg(fileNameOnly), progress);
    return true;
}

void Importer::scanDirectory(QDir const& dir)
{
   m_result.clear();
   QStack<QString> stack;
   stack.push(dir.absolutePath());
   while (!stack.isEmpty()) {
      QString sSubdir = stack.pop();
#ifdef DETAILED_LOG
      qDebug().nospace();
      qDebug() << "Importing " << sSubdir << " (stack: " << stack.size() << ")";
      qDebug().space();
#endif
      QDir subdir(sSubdir);
      QStringList entries = subdir.entryList(m_filters,
                                             QDir::Files);
#ifdef DETAILED_LOG
      for (int i = 0; i < entries.size(); i++) {
         qDebug() << "Found" << entries[i];
      }
#endif
      m_result += entries;

      QFileInfoList infoEntries = subdir.entryInfoList(QStringList(),
                                                       QDir::AllDirs | QDir::NoDotAndDotDot);
      for (int i = 0; i < infoEntries.size(); i++) {
         QFileInfo& item = infoEntries[i];
         stack.push(item.absoluteFilePath());
      }
   }
}

void Importer::destroyProgressUI() {
    if(m_progress != NULL) {
        m_progress->cancel();
        delete m_progress;
        m_progress = NULL;
    }
}

void Importer::createProgressUI() {
    destroyProgressUI();
    m_progress = new SystemProgressDialog(0);
    m_progress->setModality(SystemUiModality::Application);
    m_progress->setState(SystemUiProgressState::Active);
    m_progress->setTitle(tr("Importing Tracker Songs"));
    m_progress->confirmButton()->setEnabled(false);
    m_progress->setProgress(INDEFINITE);
    m_progress->show();
}

void Importer::updateProgressUI(QString const& body, int progress) {
#ifdef DETAILED_LOG
    qDebug() << body;
#endif
    if(m_progress) {
        m_progress->setBody(body);
        m_progress->setProgress(progress);
        m_progress->show();
    }
}
