#include "modplug/modplug.h"
#include "Importer.hpp"
#include "FileUtils.hpp"
#include "SongExtendedInfo.hpp"
#include "SongFormat.hpp"
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
    m_result.clear();

    createProgressUI();

    updateProgressUI(tr("Searching for tracker songs in shared files..."), INDEFINITE);
    scanDirectory(QDir("/accounts/1000/shared/documents"));
    scanDirectory(QDir("/accounts/1000/shared/downloads"));
    scanDirectory(QDir("/accounts/1000/shared/music"));

    updateProgressUI(tr("Searching for tracker songs in Box..."), INDEFINITE);
    scanDirectory(QDir("/accounts/1000/shared/Box"));

    updateProgressUI(tr("Searching for tracker songs in Dropbox..."), INDEFINITE);
    scanDirectory(QDir("/accounts/1000/shared/Dropbox"));

    updateProgressUI(tr("Searching for tracker songs on removable SD card..."), INDEFINITE);
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

    completeProgressUI();
}

bool Importer::importFile(QString const& fileName, int progress)
{
    QString fileNameOnly = FileUtils::fileNameOnly(fileName);
    updateProgressUI(QString(tr("Importing %1")).arg(fileNameOnly), progress);

    QFile inputFile(fileName);
    if(!inputFile.exists()) {
        return false;
    }

    if(!inputFile.open(QFile::ReadOnly)) {
        qDebug() << "Failed to open input file" << fileName;
        return false;
    }

    QByteArray data = inputFile.readAll();
    if(data.size() == 0) {
        qDebug() << "Failed to read file" << fileName;
        return false;
    }

    ModPlugFile* module = ::ModPlug_Load(data.data(), data.size());
    if(module == NULL) {
        qDebug() << "Failed to load module" << fileName;
        return false;
    }

    SongExtendedInfo info(NULL);
    info.setId(0);
    info.setFileName(fileName);
    info.setFileSize(data.size());
    info.setSongLength(::ModPlug_GetLength(module));
    info.setTitle(::ModPlug_GetName(module));
    info.setFormatId(SongFormat::getFormatIdByFileName(fileName));

    info.setInstruments(::ModPlug_NumInstruments(module));
    info.setChannels(::ModPlug_NumChannels(module));
    info.setSamples(::ModPlug_NumSamples(module));
    info.setPatterns(::ModPlug_NumPatterns(module));
    info.setOrders(::ModPlug_NumOrders(module));

    info.setFormat("");
    info.setTracker("");
    info.setGenre("");
    info.setArtistId(0);
    info.setArtist("");

    ::ModPlug_Unload(module);
    return true;
}

void Importer::scanDirectory(QDir const& dir)
{
   QStack<QString> stack;
   stack.push(dir.absolutePath());
   while (!stack.isEmpty()) {
      QString sSubdir = stack.pop();
#ifdef DETAILED_LOG
      qDebug() << "Scanning " << sSubdir << " (stack: " << stack.size() << ")";
#endif
      QDir subdir(sSubdir);
      QStringList entries = subdir.entryList(m_filters,
                                             QDir::Files);

      for (int i = 0; i < entries.size(); i++) {
          QString absoluteFileName = FileUtils::joinPath(sSubdir, entries[i]);
          m_result << absoluteFileName;
#ifdef DETAILED_LOG
          qDebug() << "Found" << absoluteFileName;
#endif
      }

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

void Importer::completeProgressUI() {
    if(m_progress) {
        m_progress->confirmButton()->setEnabled(true);
        m_progress->exec();
    }
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
