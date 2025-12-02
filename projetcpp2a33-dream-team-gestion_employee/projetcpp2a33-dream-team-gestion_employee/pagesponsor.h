#ifndef PAGESPONSOR_H
#define PAGESPONSOR_H

#include <QDialog>

namespace Ui {
class pagesponsor;
}

class pagesponsor : public QDialog
{
    Q_OBJECT

public:
    explicit pagesponsor(QWidget *parent = nullptr);
    ~pagesponsor();

private:
    Ui::pagesponsor *ui;
};

#endif // PAGESPONSOR_H
