#include "smartstudio.h"
#include "ui_smartstudio.h"

smartstudio::smartstudio(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::smartstudio)
{
    ui->setupUi(this);
}

smartstudio::~smartstudio()
{
    delete ui;
}

void smartstudio::on_pushButton_82_clicked()
{
    ui->stackedWidget->setCurrentIndex(6);
}

void smartstudio::on_pushButton_clicked()
{
     ui->stackedWidget->setCurrentIndex(1);
}


void smartstudio::on_pushButton_84_clicked()
{
       ui->stackedWidget->setCurrentIndex(5);
}


void smartstudio::on_pushButton_99_clicked()
{
       ui->stackedWidget->setCurrentIndex(1);
}


void smartstudio::on_pushButton_85_clicked()
{
       ui->stackedWidget->setCurrentIndex(4);
}


void smartstudio::on_pushButton_98_clicked()
{
       ui->stackedWidget->setCurrentIndex(1);
}


void smartstudio::on_pushButton_86_clicked()
{
       ui->stackedWidget->setCurrentIndex(3);
}


void smartstudio::on_pushButton_113_clicked()
{
       ui->stackedWidget->setCurrentIndex(1);
}


void smartstudio::on_pushButton_101_clicked()
{
       ui->stackedWidget->setCurrentIndex(2);
}


void smartstudio::on_pushButton_62_clicked()
{
       ui->stackedWidget->setCurrentIndex(1);
}


void smartstudio::on_pushButton_80_clicked()
{
       ui->stackedWidget->setCurrentIndex(1);
}

