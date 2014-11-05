#include "dialogd.h"
#include "ui_dialogd.h"
#include <QDesktopWidget>
#include "mainwindow.h"
#include "ui_mainwindow.h"

Dialogd::Dialogd(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialogd)
{
    ui->setupUi(this);
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move(screen.center() - this->rect().center());
    ui->status_label->setText(tr("Successfully decrypted file!"));
}

Dialogd::~Dialogd()
{
    delete ui;
}
