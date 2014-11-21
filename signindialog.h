#ifndef SIGNINDIALOG_H
#define SIGNINDIALOG_H

#include <QDialog>
#include <dialog.h>
#include <email.h>
#include <string>

namespace Ui {
class signinDialog;
}

class signinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit signinDialog(QWidget *parent = 0);
    ~signinDialog();
    std::string hostName;
    void setVars(std::string host);
    Dialog status;
    email messageInfo;

private slots:
    void on_signin_button_clicked();

    void closeDia();

    void on_password_lineEdit_textChanged(const QString &arg1);

private:
    Ui::signinDialog *ui;
};

#endif // SIGNINDIALOG_H
