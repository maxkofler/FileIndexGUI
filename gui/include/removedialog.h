#ifndef REMOVEDIALOG_H
#define REMOVEDIALOG_H

#include <QDialog>
#include <QThread>

#include "fs.h"

namespace Ui {
class RemoveDialog;
}

class RemoveThread : public QThread{
    Q_OBJECT
public:
    RemoveThread(FS& fs, uint64_t id);
    void run() override;
private:
    FS& _fs;
    uint64_t _id;
};

class RemoveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoveDialog(FS& fs, QWidget *parent = nullptr);
    ~RemoveDialog();

    void setTarget(uint64_t id);

private:
    Ui::RemoveDialog *ui;
    FS&                 _fs;
    uint64_t            _targetID;
    RemoveThread*       _rmThread;

private slots:
    void                remove_done();

};

#endif // REMOVEDIALOG_H
