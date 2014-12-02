#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    void success(int type);
    void fnFound();
    void smtpError();
    void netError();
    void deError();
    void enError();


signals:
    void textChanged(const QString&);

private slots:
    void on_status_label_linkActivated(const QString &link);

    void on_Dialog_accepted();

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
