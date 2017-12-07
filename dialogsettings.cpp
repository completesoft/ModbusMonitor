#include "dialogsettings.h"
#include "ui_dialogsettings.h"

#include <QColorDialog>
#include <QDebug>

DialogSettings::DialogSettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSettings)
{
    ui->setupUi(this);

    ui->twAddr->setColumnCount(3);
    ui->twAddr->setShowGrid(true);
    ui->twAddr->setHorizontalHeaderLabels(QStringList() << "Параметр" << "Описание" << "Адрес");

    ui->twAddr->insertRow(0);
    ui->twAddr->setItem(0,0, new QTableWidgetItem("temp1"));
    ui->twAddr->setItem(0,1, new QTableWidgetItem("Температура 1"));
    ui->twAddr->setItem(0,2, new QTableWidgetItem("0"));


    ui->listWidget->addItems(QStringList() << "Температура 1");


}

DialogSettings::~DialogSettings()
{
    delete ui;
}

void DialogSettings::on_pushButton_clicked()
{
    QColorDialog dlg;

    qDebug() << dlg.getColor().name();
}
