#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <enigma_ecies.hpp>
#include <icryptosystem.hpp>
#include <dialog.h>
#include <dialogd.h>

namespace Ui {
class MainWindow;

}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QString keyfile;
    QString fileName;
    QString dirIn;
    QString dirOut;
    QString userName;
    QString emailDir;
    Dialog estatus;
    Dialogd dstatus;
    ICryptosystem * key;
    bool encrypt;

private slots:
    void on_input_button_clicked();

    void on_output_button_clicked();

    void on_encrypt_button_clicked();

    void on_decrypt_button_clicked();

    void on_email_button_clicked();

    void on_input_button2_clicked();


private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
