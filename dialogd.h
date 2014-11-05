#ifndef DIALOGD_H
#define DIALOGD_H

#include <QDialog>

namespace Ui {
class Dialogd;
}

class Dialogd : public QDialog
{
    Q_OBJECT

public:
    explicit Dialogd(QWidget *parent = 0);
    ~Dialogd();

private:
    Ui::Dialogd *ui;
};

#endif // DIALOGD_H
