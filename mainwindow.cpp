#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <iostream>
#include <Poco/Net/MailMessage.h>
#include <Poco/Net/MailRecipient.h>
#include <Poco/Net/SecureStreamSocket.h>
#include <Poco/Net/SMTPClientSession.h>
#include <Poco/Net/NetException.h>
#include <Poco/Net/Context.h>
#include <Poco/Net/SSLManager.h>
#include <Poco/Net/AcceptCertificateHandler.h>
#include <Poco/AutoPtr.h>

using namespace std;
using namespace Poco::Net;
using namespace Poco;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{

    delete ui;
}

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

void MainWindow::on_output_button_clicked()
{
    dirOut = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "/home",
                                                     tr("All Files (**)"));
    if (!dirOut.isNull()) {
         ui->dirOut_lineEdit->setText(dirOut);
    }
}

void MainWindow::on_encrypt_button_clicked()
{
    userName = ui->recipient_lineEdit->text();
    key = newECIESPrivateKey();
    keyfile = "keyfile";
    key->save(keyfile.toStdString());
    key->encrypt(dirIn.toStdString(), dirOut.toStdString());
    key->destroy();
}

void MainWindow::on_decrypt_button_clicked()
{
    userName = ui->recipient_lineEdit->text();
    key = newECIESPrivateKey();
    key->load(keyfile.toStdString());
    key->decrypt(dirIn.toStdString(), dirOut.toStdString());
    key->destroy();
}

void MainWindow::on_email_button_clicked()
{
        string host = (ui->hostname_lineEdit->text()).toStdString();
        UInt16 port = 25;
        string user = (ui->username_lineEdit->text()).toStdString();
        string password = (ui->password_lineEdit->text()).toStdString();
        string to = (ui->recipient_lineEdit2->text()).toStdString();
        string from = (ui->username_lineEdit->text()).toStdString();
        string subject = "";
        string theFile = (ui->emailDir_lineEdit->text()).toStdString();
        subject = MailMessage::encodeWord(subject, "UTF-8");
        string content = "";
        MailMessage message;
        //message.addAttachment(theFile, new FilePartSource(theFile));
        message.setSender(from);
        message.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
        message.setSubject(subject);
        message.setContentType("text/plain; charset=UTF-8");
        message.setContent(content, MailMessage::ENCODING_8BIT);
        try {
            SMTPClientSession session(host, port);
            session.open();
            try {
                session.login(SMTPClientSession::AUTH_LOGIN, user, password);
                session.sendMessage(message);
                cout << "Message successfully sent" << endl;
                session.close();
            } catch (SMTPException &e) {
                session.close();
            }
        } catch (NetException &e) {

        }
}

void MainWindow::on_input_button2_clicked()
{
    emailDir = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                     "/home",
                                                     tr("All Files (**)"));
    if (!emailDir.isNull()) {
         ui->emailDir_lineEdit->setText(emailDir);
    }
}
