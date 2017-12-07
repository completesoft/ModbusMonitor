#include "log.h"
#include "ui_log.h"

#include <QDateTime>
#include <QDebug>

Log::Log(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Log)
{
    ui->setupUi(this);
}

Log::~Log()
{
    delete ui;
}

void Log::addMessage(const QString &message)
{
    QDateTime now = QDateTime::currentDateTime();

    ui->logText->appendHtml(QString(now.toString("yyyy-MM-dd hh:mm:ss")+"> %1").arg(message));

}

void Log::addError(const QString &message)
{
    this->addMessage(QString("<font color='red'>%1</font>").arg(message));

}

void Log::rotate(const QString &fileName)
{

}

void Log::addVectorData(QVector<qint16> data)
{

    if (!ui->cbRegisters->isChecked()) {
        return;
    }


    QString temp;
    for (int i=0; i<data.count(); i++) {
        temp += "<font color='green'>" + QString::number(i) + ": </font>" + QString::number(data.at(i)) + ", ";
    }

    this->addMessage("Прочитано "+temp);

}

void Log::saveToFile(const QString &fileName)
{
    qDebug() << ui->logText->toPlainText();

}


