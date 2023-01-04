#ifndef __FSTREEMODEL_H__
#define __FSTREEMODEL_H__

#include <QAbstractItemModel>

#include "fstypes.h"

class FSTreeModel : public QAbstractItemModel {
    Q_OBJECT

public:
    FSTreeModel(QObject *parent = nullptr){
        _rootDir = new FSDir(nullptr, "0", "root", "0", "0");
    }
    ~FSTreeModel(){
        delete _rootDir;
    }

    QVariant                        data(const QModelIndex &index, int role) const override;
    QVariant                        headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
    QModelIndex                     index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex                     parent(const QModelIndex &index) const override;
    int                             rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int                             columnCount(const QModelIndex &parent = QModelIndex()) const override;

    /**
     * @brief   Updates the root of the FSTree view
     * @param   root                The new root
     */
    void                            setRoot(const FSDir &root);

    /**
     * @brief   Clears all the entries from the root
     */
    void                            clearRoot();

private:
    //The root of the tree
    FSDir*                          _rootDir;

    //Utillity for getting a FSEntry* from a QModelIndex
    FSEntry                         *getItem(const QModelIndex &index) const;
};

#endif
