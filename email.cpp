#include "email.h"
#include "ui_email.h"
#include <QFileDialog>

email::email(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::email)
{
    ui->setupUi(this);
}

email::~email()
{
    delete ui;
}

void email::on_recipient_lineEdit_textChanged(const QString &arg1)
{
    recipient = ui->recipient_lineEdit->text().toStdString();
}

void email::on_subject_lineEdit_textChanged(const QString &arg1)
{
    subject = ui->subject_lineEdit->text().toStdString();
}

void email::on_body_textEdit_textChanged()
{
    body = ui->body_textEdit->toPlainText().toStdString();
}

std::string email::getTo() {
    return recipient;
}

std::string email::getSubject() {
    return subject;
}

std::string email::getBody() {
    return body;
}

std::string email::getFile() {
    std::string theFile = emailDir.toStdString();
    return theFile;
}

void email::on_dir_button_clicked()
{
    emailDir = QFileDialog::getOpenFileName(this, tr("Open File"),
                                            "/home",
                                            tr("All Files (**)"));
    if (!emailDir.isNull()) {
        ui->file_lineEdit->setText(emailDir);
    }
}

void email::on_send_clicked()
{
    this->close();
}
