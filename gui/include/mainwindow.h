#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QProgressDialog>
#include <QListView>
#include <QTreeView>

#include <deque>

#include "fstypes.h"
#include "indexdialog.h"
#include "settingsdialog.h"
#include "indexthread.h"
#include "searchmanager.h"
#include "stats.h"
#include "fstreemodel.h"

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

    SQL                         _sql;
    FS                          _fs;
    FileIndex                   _index;
    SearchManager               _searchManager;

    //The objects for the fileindex functionality
    IndexThread*                _indexThread = nullptr;
    QMetaObject::Connection     _c_indexThread;

    //Objects for results list
    QTreeView*                  _tv_results = nullptr;
    FSTreeModel                 _m_results;

    stats_search                _search_stats;

    IndexDialog                 _dialog;
    SettingsDialog              _dialog_settings;

    /**
     * @brief   If the search should match the cases
     */
    bool                        _search_matchCase = false;

    /**
     * @brief   If the filesystem tree should expand automatically when searching
     */
    bool                        _search_autoExpand = true;

private slots:
    /**
     * @brief   Gets called a new index should be added
     */
    void                        onNewIndex();

    /**
     * @brief   Gets called when the database should be exported
     */
    void                        onActionExport();

    /**
     * @brief   Gets called when a database should be imported
     */
    void                        onActionImport();

    /**
     * @brief   Tells the main window a new fs entry has been found, updates the status bar
     * @param   path            The path of the entry
     * @param   isDir           If it is a directory
     */
    void                        onIndexFound(const QString& path, bool isDir);

    /**
     * @brief   Gets called once the indexing dialog is done
     */
    void                        onIndexDialogDone(int);

    /**
     * @brief   Notifies the class that the indexing process has been finished and the indexer can be cleaned up
     */
    void                        onIndexDone();

    /**
     * @brief   If the text in the search bar has changed, triggers a new search
     * @param   text            The new text in the bar
     */
    void                        onTeTextChanged(const QString& text);

    /**
     * @brief   Gets called from the searchManager once a result is incoming
     * @param   searchTerm      The string searched for
     * @param   res             A FSDir as the root of all results
     * @param   us_searched     The us needed for finding the results
     */
    void                        onSearchResult(std::string searchTerm, FSDir res, uint64_t us_searched);

    /**
     * @brief   Gets called every time the user double-clicked a result
     * @param   index           The position the user clicked
     */
    void                        onResultDoubleClicked(const QModelIndex& index);

public slots:

    /**
     * @brief   Urges the mainwindow to reload its settings
     */
    void                        reloadSettings();

};
#endif // MAINWINDOW_H
