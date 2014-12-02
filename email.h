#ifndef EMAIL_H
#define EMAIL_H

#include <QDialog>
#include <string>

namespace Ui {
class email;
}

class email : public QDialog
{
    Q_OBJECT

public:
    explicit email(QWidget *parent = 0);
    std::string recipient;
    std::string subject;
    std::string body;
    QString emailDir;
    std::string getTo();

    std::string getSubject();

    std::string getBody();

    std::string getFile();

    ~email();

private slots:
    void on_recipient_lineEdit_textChanged(const QString &arg1);

    void on_subject_lineEdit_textChanged(const QString &arg1);

    void on_body_textEdit_textChanged();

    void on_dir_button_clicked();

    void on_send_clicked();

private:
    Ui::email *ui;
};

#endif // EMAIL_H
