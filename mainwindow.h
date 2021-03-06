#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <enigma_ecies.hpp>
#include <icryptosystem.hpp>
#include <dialog.h>
#include <signindialog.h>
#include "fstream"
#include <userdialog.h>
#include <QFile>
#include <QtHelp/QtHelp>
#include <sqlwrapper.h>

using namespace std;

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    string keyfile; //keyfile
    QString fileName; //
    QString dirIn; //directory for encryption/decryption input file
    QString dirOut; //directory for encryption/decryption output file
    QString userName; //
    QString emailDir; //directory for file to be emailed
    Dialog status; //status window
    ICryptosystem * key; //virtual cryptosystem containing private key
    signinDialog signin;
    QString userFileName;
    QFile userFile;
    userDialog u;
    string host; //hostname string for outgoing mail server
    bool encrypt;
    mysqlpp::Connection * keyserver;

private slots:
    void on_input_button_clicked();

    void on_output_button_clicked();

    void on_encrypt_button_clicked();

    void on_decrypt_button_clicked();

    void on_email_button_clicked();

    //void on_password_lineEdit_textChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(const QString &arg1);


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
