#include "signindialog.h"
#include "ui_signindialog.h"
#include "mainwindow.h"
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

signinDialog::signinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::signinDialog)
{
    ui->setupUi(this);
}

signinDialog::~signinDialog()
{
    delete ui;
}

void signinDialog::on_signin_button_clicked()
{
    UInt16 port = 587;
    string user = ui->username_lineEdit->text().toStdString();
    string password = ui->password_lineEdit->text().toStdString();
    status.setModal(true);
    try {
        initializeSSL();
        SharedPtr<InvalidCertificateHandler> ptrHandler = new AcceptCertificateHandler(false);
        Context::Ptr ptrContext = new Context(Context::CLIENT_USE, "", "", "", Context::VERIFY_RELAXED, 9, true, "ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
        SSLManager::instance().initializeClient(0, ptrHandler, ptrContext);

        SecureSMTPClientSession theSession(hostName, port);

        try {
            theSession.login();
            if (theSession.startTLS()) {
                theSession.login(SMTPClientSession::AUTH_LOGIN, user, password);
                messageInfo.setModal(true);
                messageInfo.exec();
                MailMessage theMessage;
                string to = messageInfo.getTo();
                string subject = messageInfo.getSubject();
                string body = messageInfo.getBody();
                string theFile = messageInfo.getFile();

                try {
                       theMessage.addAttachment("the File", new FilePartSource(theFile));
                } catch (Poco::FileNotFoundException){
                    status.fnFound();
                }

                theMessage.setSender(user);
                theMessage.addRecipient(MailRecipient(MailRecipient::PRIMARY_RECIPIENT, to));
                theMessage.setSubject(subject);
                theMessage.setContentType("text/plain; charset=UTF-8");
                theMessage.setContent(body, MailMessage::ENCODING_8BIT);

                theSession.sendMessage(theMessage);
                status.success(2);
            }
            theSession.close();
            uninitializeSSL();
        } catch (SMTPException &e) {
            theSession.close();
            uninitializeSSL();
            status.smtpError();
        }
    }
    catch (NetException &e) {
        status.netError();
    }
    closeDia();
    status.exec();
}

void signinDialog::setVars(string host) {
    hostName = host;
}

void signinDialog::on_password_lineEdit_textChanged(const QString &arg1)
{
    ui->password_lineEdit->setEchoMode(QLineEdit::Password);
}

void signinDialog::closeDia() {
    this->close();
}
