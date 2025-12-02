#include "pagesponsor.h"
#include "ui_pagesponsor.h"

pagesponsor::pagesponsor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pagesponsor)
{
    ui->setupUi(this);
}

pagesponsor::~pagesponsor()
{
    delete ui;
}
