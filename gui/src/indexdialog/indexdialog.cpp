#include "indexdialog.h"
#include "../../ui/ui_indexdialog.h"

IndexDialog::IndexDialog(QWidget *parent) : QDialog{parent}, ui(new Ui::IndexDialog)
{
    ui->setupUi(this);
}
