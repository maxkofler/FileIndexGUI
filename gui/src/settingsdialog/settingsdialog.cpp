#include "log.h"
#include "settingsdialog.h"
#include "../../ui/ui_settingsdialog.h"

#include <QFileDialog>
#include <QSettings>

SettingsDialog::SettingsDialog(QWidget *parent) : QDialog(parent), ui(new Ui::SettingsDialog){
    FUN();
    ui->setupUi(this);

    QSettings s;
    ui->le_startupdb_path->setText(s.value("startup/dbfile", "").toString());

    connect(ui->bt_startupdb_path, &QPushButton::clicked, this, &SettingsDialog::onSelectStartupDB);

    connect(ui->bt_done, &QPushButton::clicked, this, &SettingsDialog::hide);
}

SettingsDialog::~SettingsDialog(){
    FUN();
    delete ui;
}

//
// SLOTS
//

void SettingsDialog::onSelectStartupDB(){
    QFileDialog fDialog(this, tr("Select the database to load on startup"), ui->le_startupdb_path->text());
    fDialog.setFileMode(QFileDialog::ExistingFile);
    fDialog.setNameFilter("FileIndex Database (*.fidb)");
    fDialog.exec();
    if (fDialog.selectedFiles().size() == 0){
        return;
    }
    std::string path = fDialog.selectedFiles().at(0).toStdString();
    LOGU("[SettingsDialog][onSelectStartupDB] Selected " + path + " as the startup database");

    ui->le_startupdb_path->setText(QString::fromStdString(path));
}

void SettingsDialog::hide(){
    FUN();

    LOGU("[SettingsDialog][hide] Saving preferences...");

    QSettings s;

    s.setValue("startup/dbfile", ui->le_startupdb_path->text());

    QDialog::hide();
}
