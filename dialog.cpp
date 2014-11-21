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

void Dialog::success(int type) {
    if (type == 0) {
        ui->status_label->setText(tr("Successfully encrypted file!"));
    } else if (type == 1) {
        ui->status_label->setText(tr("Successfully decrypted file!"));
    } else if (type == 2) {
        ui->status_label->setText(tr("Successfully emailed file!"));
    }
}

void Dialog::fnFound() {
    ui->status_label->setText(tr("Error: File not found"));
}

void Dialog::smtpError() {
    ui->status_label->setText(tr("Invalid email/password combination"));
}

void Dialog::netError() {
    ui->status_label->setText(tr("Network error"));
}
