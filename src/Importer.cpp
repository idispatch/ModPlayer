#include "modplug/modplug.h"
#include "Importer.hpp"
#include "FileUtils.hpp"
#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <bb/system/SystemProgressDialog>

using namespace bb::system;

Importer::Importer(QObject * parent)
    : QObject(parent),
      m_progress(NULL) {
}

Importer::~Importer() {
    destroyProgressUI();
}

void Importer::clean() {

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
    //m_progress->setBody(QString(tr("Creating %1")).arg(fileName));
    m_progress->setProgress(0);
    m_progress->show();
}
