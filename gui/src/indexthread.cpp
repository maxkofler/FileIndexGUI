#include "indexthread.h"

IndexThread::IndexThread(FileIndex& index, const std::string& rootPath, bool recursive, const std::string& crateName, QObject *parent) : QThread{parent}, _index(index), _rootPath(rootPath), _crateName(crateName), _recursive(recursive){

}

void IndexThread::indexFoundStatic(const std::string_view& name, size_t id, bool isDir, void* udata){
    ((IndexThread*)(udata))->indexFoundSlot(name, id, isDir);
}

void IndexThread::indexFoundSlot(const std::string_view& name, size_t id, bool isDir){
    emit indexFound(QString().fromStdString(std::string(name)), id, isDir);
}

void IndexThread::run(){
    _index.setCBFSEntryIndexed(&IndexThread::indexFoundStatic, this);
    _index.index(_rootPath, _recursive, _crateName);
    _index.removeCBFSEntryIndexed();
    emit done();
}
