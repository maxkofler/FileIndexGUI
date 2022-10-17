#include "log.h"
#include "indexdialog.h"
#include "../../ui/ui_indexdialog.h"

#include <QFileDialog>

IndexDialog::IndexDialog(QWidget *parent) : QDialog{parent}, ui(new Ui::IndexDialog)
{
    ui->setupUi(this);

    connect(ui->bt_path, &QPushButton::clicked, this, &IndexDialog::onSelectPath);
}

std::string IndexDialog::getPath(){
    return ui->le_path->text().toStdString();
}

std::string IndexDialog::getCrate(){
    return ui->le_crate->text().toStdString();
}

//
// SLOTS
//

void IndexDialog::onSelectPath(){
    QFileDialog fDialog(this, tr("Select the directory to index"), ui->le_path->text());
    fDialog.setFileMode(QFileDialog::Directory);
    fDialog.exec();
    if (fDialog.selectedFiles().size() == 0){
        return;
    }
    std::string path = fDialog.selectedFiles().at(0).toStdString();
    LOGU("[IndexDialog][onSelectPath] Selected " + path);

    ui->le_path->setText(QString::fromStdString(path));
}

