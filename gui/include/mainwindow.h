#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QProgressDialog>
#include <QListView>
#include <QStringList>
#include <QStringListModel>
#include <QFutureWatcher>

#include <deque>

#include "indexdialog.h"
#include "indexthread.h"
#include "stats.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    /**
     * @brief   Displays a new status message in the status bar
     * @param message
     * @param timeout
     */
    void                        showNewStatusMessage(const QString &message, int timeout = 0);

private:
    Ui::MainWindow *ui;

    //The objects for the fileindex functionality
    NamesDB*                    _db = nullptr;
    FS*                         _fs = nullptr;
    FileIndex*                  _index = nullptr;
    IndexThread*                _indexThread = nullptr;
    QMetaObject::Connection     _c_indexThread;

    //Objects for results list
    QListView*                  _lv_results = nullptr;
    QStringListModel*           _m_results;
    QStringList                 _sl_results;

    QFutureWatcher<std::deque<namesDB_searchRes>> _searchWatcher;

    stats_search                _search_stats;

    IndexDialog                 _dialog;

private slots:
    /**
     * @brief   Gets called a new index should be added
     */
    void                        onNewIndex();

    /**
     * @brief   Tells the main window a new fs entry has been found, updates the status bar
     * @param   path            The path of the entry
     * @param   id              The id of the entry
     * @param   isDir           If it is a directory
     */
    void                        onIndexFound(const QString& path, size_t id, bool isDir);

    /**
     * @brief   Notifies the class that the indexing process has been finished and the indexer can be cleaned up
     */
    void                        onIndexDone();

    /**
     * @brief   Notifies this class that the searching process is done
     */
    void                        onSearchDone();

    /**
     * @brief   If the text in the search bar has changed, triggers a new search
     * @param   text            The new text in the bar
     */
    void                        onTeTextChanged(const QString& text);

    /**
     * @brief   Gets called every time the user double-clicked a result
     * @param   index           The position the user clicked
     */
    void                        onResultDoubleClicked(const QModelIndex& index);

};
#endif // MAINWINDOW_H
