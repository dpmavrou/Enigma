#include "dialog.h"
#include "ui_dialog.h"
#include <QDesktopWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{

    ui->setupUi(this);
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move(screen.center() - this->rect().center());
    ui->status_label->setText(tr("Successfully encrypted file!"));
}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::on_status_label_linkActivated(const QString &link)
{
}

void Dialog::on_Dialog_accepted()
{
}
