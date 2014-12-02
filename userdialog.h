#ifndef USERDIALOG_H
#define USERDIALOG_H

#include <QDialog>
#include <string>
#include <QFile>

namespace Ui {
class userDialog;
}

class userDialog : public QDialog
{
    Q_OBJECT

public:
    explicit userDialog(QWidget *parent = 0);
    ~userDialog();
    QFile userFile;
    std::string userFileStr;
    QString username;
    QString getUserName();

private slots:
    void on_signin_button_clicked();

private:
    Ui::userDialog *ui;
};

#endif // USERDIALOG_H
