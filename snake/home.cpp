#include "home.h"
#include "ui_home.h"

#include <iostream>

Home::Home(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Home)
{
    ui->setupUi(this);
}

Home::~Home()
{
    delete ui;
}

void Home::on_pushButton_clicked()
{
    display(1);
}
