#ifndef INDEXTHREAD_H
#define INDEXTHREAD_H

#include <QThread>
#include "fileIndex.h"

class IndexThread : public QThread
{
    Q_OBJECT
public:
    explicit                IndexThread(FileIndex& index, const std::string& rootPath, bool recursive, const std::string& crateName = "", QObject *parent = nullptr);

signals:
    /**
     * @brief   Gets emited every time the indexer finds a new filesystem entry
     * @param   path        The path to the found entry
     * @param   isDir       If the entry is a directory
     */
    void                    indexFound(const QString &path, bool isDir);

    /**
     * @brief   Gets emitted once the thread is done and can be cleaned up
     */
    void                    done();

protected:
    void                    run() override;

private:
    FileIndex&              _index;
    std::string             _rootPath;
    bool                    _recursive;
    std::string             _crateName;

    /**
     * @brief   A helper method for the callback from the FileIndex class when indexing to redirect to indexFoundSlot
     * @param name
     * @param isDir
     * @param udata
     */
    static void             indexFoundStatic(const std::string_view& name, bool isDir, void* udata);

    /**
     * @brief   Gets called every time a filesystem entry has been found while indexing, used for progress
     * @param   path        The full path to the file
     * @param   isDir       If the entry is a directory
     */
    void                    indexFoundSlot(const std::string_view& path, bool isDir);

};

#endif // INDEXTHREAD_H
