#ifndef INDEXDIALOG_H
#define INDEXDIALOG_H

//TODO: Investigate this
#include "log.h"
#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class IndexDialog; }
QT_END_NAMESPACE


class IndexDialog : public QDialog
{
    Q_OBJECT
public:
    explicit IndexDialog(QWidget *pareDEBUG_EXnt = nullptr);

    std::string                 getPath();

    std::string                 getCrate();

private:
    Ui::IndexDialog *ui;

    std::string                 _path;

signals:
    void                        done();

private slots:
    void                        onSelectPath();

};

#endif // INDEXDIALOG_H
