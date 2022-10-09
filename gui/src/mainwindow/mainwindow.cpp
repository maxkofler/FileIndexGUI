#include "log.h"
#include "mainwindow.h"
#include "../../ui/ui_mainwindow.h"

#include "fileIndex.h"

#include <iostream>

#include <QFileDialog>
#include <QShortcut>
#include <QKeySequence>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);

    //Set up the results list
    _lv_results = ui->lv_results;
    _m_results = new QStringListModel(this);
    _m_results->setStringList(_sl_results);
    _lv_results->setModel(_m_results);
    _lv_results->setEditTriggers(QAbstractItemView::NoEditTriggers);

    //Create the filesystem and index
    _db = new NamesDB("FS-Main");
    _fs = new FS(_db);
    _index = new FileIndex(_fs);

    //Connect signals
    QObject::connect(ui->actionIndex, &QAction::triggered, this, &MainWindow::onNewIndex);
    QObject::connect(ui->lineEdit, &QLineEdit::textChanged, this, &MainWindow::onTeTextChanged);
    QObject::connect(this, &MainWindow::showNewStatusMessage, ui->statusbar, &QStatusBar::showMessage);
    QObject::connect(_lv_results, &QListView::doubleClicked, this, &MainWindow::onResultDoubleClicked);
}

MainWindow::~MainWindow(){
    delete ui;

    delete _m_results;

    delete _index;
    delete _fs;
    delete _db;
}

void MainWindow::onNewIndex(){
    QFileDialog fDialog(this);
    fDialog.setFileMode(QFileDialog::Directory);
    fDialog.exec();
    if (fDialog.selectedFiles().size() == 0){
        LOGU("[MainWindow][onNewIndex] No directory selected");
        return;
    }
    std::string path = fDialog.selectedFiles().at(0).toStdString();
    LOGU("[MainWindow][onNewIndex] Starting to index " + path);

    _indexThread = new IndexThread(*_index, path, true);
    _c_indexThread = QObject::connect(_indexThread, &IndexThread::indexFound, this, &MainWindow::onIndexFound, Qt::QueuedConnection);
    QObject::connect(_indexThread, &IndexThread::done, this, &MainWindow::onIndexDone, Qt::QueuedConnection);

    _indexThread->start();
}

//SLOTS

void MainWindow::onIndexFound(const QString& name, size_t id, bool isDir){
    emit showNewStatusMessage(name);
}

void MainWindow::onIndexDone(){
    QObject::disconnect(_c_indexThread);
    delete _indexThread;
    emit showNewStatusMessage("Done!");
    _db->updateIndex();
    LOGU("[MainWindow][onIndexDone] Finished indexing");
    onTeTextChanged("");
}

void MainWindow::onTeTextChanged(const QString& text){
    auto searchRes = _db->searchAll(text.toStdString(), false);
    if (searchRes.size() == 0){
        emit showNewStatusMessage(QString().fromStdString("No matches!"));
        _sl_results.clear();
        _m_results->setStringList(_sl_results);
        return;
    }

    std::sort(searchRes.begin(), searchRes.end(), [](const namesDB_searchRes &a, const namesDB_searchRes &b) -> bool {
        return a.matchRemaining < b.matchRemaining;
    });

    if (searchRes.size() > 1000){
        std::string msg = "Showing only best 1000 of " + std::to_string(searchRes.size()) + " results";
        emit showNewStatusMessage(QString().fromStdString(msg));
    } else {
        std::string msg = "Showing " + std::to_string(searchRes.size()) + " results";
        emit showNewStatusMessage(QString().fromStdString(msg));
    }

    _sl_results.clear();
    namesDB_searchRes res;
    std::string name;
    for (size_t i = 0; i < searchRes.size() && i < 1000; i++){
        res = searchRes.at(i);

        if(_fs->getEntry((size_t)res.data)->isDir)
            name = "[D] ";
        else
            name = "[F] ";

        name += _fs->getEntryPathString((size_t)res.data);
        _sl_results.append(QString().fromStdString(name));
    }
    _m_results->setStringList(_sl_results);
}

void MainWindow::onResultDoubleClicked(const QModelIndex& index){
    QVariant res = _m_results->data(index);
    std::cout << "Double clicked " << res.toString().toStdString() << std::endl;
}
