#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <QDesktopWidget>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/SecureSMTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/AutoPtr.h>
#include <Poco/Net/FilePartSource.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;
using namespace CryptoPP;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    u.exec();
    userFileName = u.getUserName();
    keyfile = userFileName.toStdString() + ".ecies";
    key = newECIESPrivateKey();
    key->load(keyfile);

    keyserver = sqlwrapper::sql_connect();
    sqlwrapper::put(userFileName.toStdString(), key->public_key_to_string(),keyserver);

    this->show();
    ui->setupUi(this);
    const QRect screen = QApplication::desktop()->screenGeometry();
    this->move(screen.center() - this->rect().center());

    //set host
    host = "smtp.gmail.com";

    //tooltips
    ui->encrypt_button->setToolTip("Encrypts selected file into output location");
    ui->decrypt_button->setToolTip("Decrypts selected file into output location");
    ui->recipient_lineEdit->setToolTip("Recipient");
    ui->dirOut_lineEdit->setToolTip("Path to output file to be used as output location for encrypt and decrypt");
    ui->dirIn_lineEdit->setToolTip("Path to input file to be encrypted or decrypted");
    ui->input_button->setToolTip("Opens directory browser to select input file");
    ui->output_button->setToolTip("Opens directory browser to select output file");
    ui->email_button->setToolTip("Press to open sign-in interface for selected mail server");
    ui->comboBox->setToolTip("The email service provider to be used");
}

MainWindow::~MainWindow()
{

    delete ui;
}

/*
 * Method to open directory window upon button press
*/
void MainWindow::on_input_button_clicked()
{
    QFileInfo fi;
    dirIn = QFileDialog::getOpenFileName(this, tr("Open File"),
                                         "/home",
                                         tr("All Files (**)"));
    fi.setFile(dirIn);
    fileName = fi.baseName();
    if (!dirIn.isNull()) {
        ui->dirIn_lineEdit->setText(dirIn);
    }
}

/*
 * Method to open directory window upon button press
*/
void MainWindow::on_output_button_clicked()
{
    dirOut = QFileDialog::getOpenFileName(this, tr("Open File"),
                                          "/home",
                                          tr("All Files (**)"));
    if (!dirOut.isNull()) {
        ui->dirOut_lineEdit->setText(dirOut);
    }
}

/*
 * Method to call cryptosystem methods to encrypt a file on button press
*/
void MainWindow::on_encrypt_button_clicked()
{
    userName = ui->recipient_lineEdit->text();
    dirIn = ui->dirIn_lineEdit->text();
    dirOut = ui->dirOut_lineEdit->text();
    try {
        ICryptosystem * tempKey = newECIESPublicKey();
        tempKey->public_key_from_string(sqlwrapper::get(userName.toStdString(),keyserver));
        tempKey->encrypt(dirIn.toStdString(), dirOut.toStdString());
        tempKey->destroy();
        status.setModal(true);
        status.success(0);
    } catch(std::exception& e){
        status.enError();
    }
    status.exec();
}

/*
 * Method to call cryptosystem methods to decrypt a file on button press
*/
void MainWindow::on_decrypt_button_clicked()
{
    dirIn = ui->dirIn_lineEdit->text();
    dirOut = ui->dirOut_lineEdit->text();
    try {
        key->decrypt(dirIn.toStdString(), dirOut.toStdString());
        status.setModal(true);
        status.success(1);
    } catch (std::exception& e) {
        status.deError();
    }
    status.exec();
}

/*
 * Method to initiate email and send email with an attached file upon button press
*/
void MainWindow::on_email_button_clicked()
{
    signin.setModal(true);
    signin.setVars(host);
    signin.exec();
}



/*void MainWindow::on_password_lineEdit_textChanged(const QString &arg1)
{
    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
}*/

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    if (ui->comboBox->currentIndex() == 0) {
        host = "smtp.gmail.com";
    } else if(ui->comboBox->currentIndex() == 1) {
        host = "smtp.mail.yahoo.com";
    } else if(ui->comboBox->currentIndex() == 2) {
        host = "smtp.live.com";
    } else if(ui->comboBox->currentIndex() == 3) {
        host = "smtp.mail.com";
    } else if(ui->comboBox->currentIndex() == 4) {
        host = "smtp.yandex.com";
    } else if(ui->comboBox->currentIndex() == 5) {
        host = "smtp.hushmail.com";
    } else if(ui->comboBox->currentIndex() == 6) {
        host = "smtp.aim.com";
    } else if(ui->comboBox->currentIndex() == 7) {
        host = "smtp.shortmail.com";
    } else if(ui->comboBox->currentIndex() == 8) {
        host = "smtp.mail.me.com";
    }
    else {
        //I like ponies
    }
}
