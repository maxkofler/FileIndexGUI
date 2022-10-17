#ifndef INDEXDIALOG_H
#define INDEXDIALOG_H

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class IndexDialog; }
QT_END_NAMESPACE


class IndexDialog : public QDialog
{
    Q_OBJECT
public:
    explicit IndexDialog(QWidget *parent = nullptr);

private:
    Ui::IndexDialog *ui;

signals:

};

#endif // INDEXDIALOG_H
