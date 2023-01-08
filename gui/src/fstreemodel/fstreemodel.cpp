#include "log.h"
#include "fstreemodel.h"

FSTreeModel::FSTreeModel(QObject *parent) : QAbstractItemModel(parent){
    FUN();
    _rootDir = new FSDir(nullptr, "0", "root", "0", "0");
}

FSTreeModel::~FSTreeModel(){
    FUN();
    delete _rootDir;
}

QVariant FSTreeModel::data(const QModelIndex &index, int role) const {
    FUN();

    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    FSEntry* item = getItem(index);
    return QVariant(item->name.c_str());
}

QVariant FSTreeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    FUN();

    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return QVariant("Name");

    return QVariant();
}

QModelIndex FSTreeModel::index(int row, int column, const QModelIndex &parent) const {
    FUN();

    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    FSEntry* parentItem = getItem(parent);
    if (!parentItem)
        return QModelIndex();

    FSEntry* childItem = parentItem->child((size_t)row).get();
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}

QModelIndex FSTreeModel::parent(const QModelIndex &index) const {
    FUN();

    if (!index.isValid())
        return QModelIndex();

    FSEntry* childItem = getItem(index);
    FSEntry* parentItem = childItem ? childItem->parent : nullptr;

    if (parentItem == _rootDir || !parentItem)
        return QModelIndex();

    return createIndex((int)parentItem->childNumber(), 0, parentItem);
}


int FSTreeModel::rowCount(const QModelIndex &parent) const {
    FUN();

    if (parent.isValid() && parent.column() > 0)
        return 0;

    FSEntry *pI = getItem(parent);

    return pI ? pI->childCount() : 0;
}

int FSTreeModel::columnCount(const QModelIndex &parent) const {
    FUN();

    Q_UNUSED(parent);
    return 1;
}

void FSTreeModel::setRoot(const FSDir &root){
    FUN();

    *_rootDir = root;
    _rootDir->sortChildrenRec();
    emit layoutChanged();
}

void FSTreeModel::clearRoot(){
    FUN();

    _rootDir->children.clear();
    emit layoutChanged();
}

FSEntry* FSTreeModel::getItem(const QModelIndex &index) const {
    if (index.isValid()){
        FSEntry* item = static_cast<FSEntry*>(index.internalPointer());
        if (item)
            return item;
    }
    return _rootDir;
}

