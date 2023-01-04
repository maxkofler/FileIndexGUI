#include "log.h"
#include "mainwindow.h"
#include "../../ui/ui_mainwindow.h"

#include "fileIndex.h"
#include "indexdialog.h"

#include <iostream>
#include <chrono>
#include <fstream>

#include <QFileDialog>
#include <QShortcut>
#include <QKeySequence>

#include <QFuture>
#include <QFutureWatcher>
#include <QtConcurrent/QtConcurrentRun>

#include <QSettings>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  _dialog(this),
  _dialog_settings(this),
  _sql(),
  _fs(_sql),
  _index(&_fs),
  _searchManager(_fs)
{

    FUN();
    ui->setupUi(this);

    QSettings s;

    //Setup subdialogs
    _dialog.setWindowModality(Qt::WindowModality::WindowModal);
    _dialog_settings.setWindowModality(Qt::WindowModality::WindowModal);

    //Set up the results tree
    _tv_results = ui->tv_results;
    _tv_results->setModel(&_m_results);

    //Connect signals
    QObject::connect(ui->actionIndex, &QAction::triggered, this, &MainWindow::onNewIndex);
    QObject::connect(ui->actionExport, &QAction::triggered, this, &MainWindow::onActionExport);
    QObject::connect(ui->actionImport, &QAction::triggered, this, &MainWindow::onActionImport);
    QObject::connect(ui->actionSettings, &QAction::triggered, &_dialog_settings, &QDialog::show);

    QObject::connect(&_dialog, &IndexDialog::finished, this, &MainWindow::onIndexDialogDone);
    QObject::connect(ui->le_search, &QLineEdit::textChanged, this, &MainWindow::onTeTextChanged);
    QObject::connect(this, &MainWindow::showNewStatusMessage, ui->statusbar, &QStatusBar::showMessage);
    QObject::connect(&_dialog_settings, &QDialog::accepted, this, &MainWindow::reloadSettings);

    QObject::connect(&_searchManager, &SearchManager::resultReady, this, &MainWindow::onSearchResult, Qt::QueuedConnection);

    //Import startup database
    QString startupDB = s.value("startup/dbfile", "").toString();
    if (startupDB != ""){
        _sql.load(startupDB.toStdString());
        _fs.createTables();
    }

    //Update the contents
    onTeTextChanged(ui->le_search->text());

    //Load MainWindow settings
    reloadSettings();
}

MainWindow::~MainWindow(){
    FUN();

    delete ui;
}

//
// SLOTS
//

void MainWindow::onNewIndex(){
    FUN();

    _dialog.show();
}

void MainWindow::onActionExport(){
    FUN();

    LOGU("[MainWindow][export]");
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save FileIndex Database"), "db.fidb", tr("FileIndex Database (*.fidb)"));
    std::string path = fileName.toStdString();
    LOGU("[MainWindow][export] Saving database to " + path);

    if (!_sql.save(path))
        LOGUE("[MainWindow][export] Failed to open file " + path + " for writing");

}

void MainWindow::onActionImport(){
    FUN();

    LOGU("[MainWindow][import]");
    QFileDialog fDialog(this, tr("Select the file to import"));
    fDialog.setFileMode(QFileDialog::ExistingFile);
    fDialog.setNameFilter("FileIndex Database (*.fidb)");
    fDialog.exec();
    if (fDialog.selectedFiles().size() == 0){
        return;
    }
    std::string path = fDialog.selectedFiles().at(0).toStdString();
    LOGU("[MainWindow][importing] Importing database from " + path);

    if (!_sql.load(path))
        LOGUE("[MainWindow][import] Failed to open file " + path + " for reading");

    _fs.createTables();

    onTeTextChanged(ui->le_search->text());
}

void MainWindow::onIndexDialogDone(int){
    FUN();

    std::string path = _dialog.getPath();
    std::string crate = _dialog.getCrate();
    LOGU("[MainWindow][onIndexDialogDone] Indexing " + path + "...");
    _indexThread = new IndexThread(_index, path, true, crate);
    _c_indexThread = QObject::connect(_indexThread, &IndexThread::indexFound, this, &MainWindow::onIndexFound, Qt::QueuedConnection);
    QObject::connect(_indexThread, &IndexThread::done, this, &MainWindow::onIndexDone, Qt::QueuedConnection);

    _indexThread->start();
    ui->le_search->setDisabled(true);
}

void MainWindow::onIndexFound(const QString& name, bool isDir){
    FUN();

    emit showNewStatusMessage(name);
}

void MainWindow::onIndexDone(){
    FUN();

    QObject::disconnect(_c_indexThread);
    delete _indexThread;
    emit showNewStatusMessage("Done!");
    LOGU("[MainWindow][onIndexDone] Finished indexing");
    onTeTextChanged("");
    ui->le_search->setDisabled(false);
}

void MainWindow::onTeTextChanged(const QString& text){
    FUN();

    //Order a new search action
    _searchManager.search(text.toStdString(), _search_matchCase);
}

void MainWindow::onSearchResult(std::string searchTerm, FSDir res, uint64_t us_searched){
    FUN();

    //LOGU("[MainWindow][onSearchResult] Got " + std::to_string(res.size()) + " results from search term '" + searchTerm + "'");

    /*if (res.size() == 0){
        emit showNewStatusMessage(QString().fromStdString("No matches!"));
        //_sl_results.clear();
        //_ms_results->setStringList(_sl_results);
        return;
    }

    {
        std::string logMsg;
        if (res.size() > 1000){
            logMsg = "Showing 1000 of " + std::to_string(res.size()) + " results";

        } else {
            logMsg = "Showing " + std::to_string(res.size()) + " results";
        }

        logMsg += ", searching took " + std::to_string(us_searched) + " us";
        logMsg += ", sorting took " + std::to_string(_search_stats.us_sort) + " us";

        emit showNewStatusMessage(QString().fromStdString(logMsg));
    }*/

    _m_results.setRoot(res);
}

void MainWindow::onResultDoubleClicked(const QModelIndex& index){
    FUN();

    QVariant res = _m_results->data(index);
    std::cout << "Double clicked " << res.toString().toStdString() << std::endl;
}

void MainWindow::reloadSettings(){
    FUN();

    LOGU("[MainWindow] Reloading settings...");

    QSettings s;

    _search_matchCase = s.value("search/matchCase", false).toBool();

    onTeTextChanged(ui->le_search->text());
}
