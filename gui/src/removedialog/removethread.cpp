
#include "removedialog.h"

RemoveThread::RemoveThread(FS& fs, uint64_t id) : _fs(fs), _id(id){

}

void RemoveThread::run(){
    fs_entry e;
    e.id = _id;
    _fs.remove(e);
}
