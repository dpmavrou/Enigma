#include "userdialog.h"
#include "ui_userdialog.h"
#include "enigma_ecies.hpp"
#include <icryptosystem.hpp>
#include <QFile>

using namespace std;

userDialog::userDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::userDialog)
{
    ui->setupUi(this);
}

userDialog::~userDialog()
{
    delete ui;
}

void userDialog::on_signin_button_clicked()
{
    username = ui->username_lineEdit->text();
    userFileStr = username.toStdString() + ".ecies";
    userFile.setFileName(username + ".ecies");
    if (!userFile.exists()) {
        userFile.open(QIODevice::WriteOnly);
        userFile.close();
        ICryptosystem * tempkey = newECIESPrivateKey();
        tempkey->save(userFileStr);
        tempkey->destroy();
    }
    this->close();
    // file = username.ecies
}

QString userDialog::getUserName() {
    return username;
}
