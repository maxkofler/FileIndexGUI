#include "removedialog.h"
#include "../../ui/ui_removedialog.h"

RemoveDialog::RemoveDialog(FS& fs, QWidget *parent) :
    QDialog(parent),
    _fs(fs),
    ui(new Ui::RemoveDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::WindowType::SplashScreen);
}

RemoveDialog::~RemoveDialog()
{
    delete ui;
}

void RemoveDialog::setTarget(uint64_t id){
    _rmThread = new RemoveThread(_fs, id);
    connect(_rmThread, &QThread::finished, this, &RemoveDialog::remove_done);
    _rmThread->start();
}

//
// SLOTS
//

void RemoveDialog::remove_done(){
    delete _rmThread;
    done(0);
}
