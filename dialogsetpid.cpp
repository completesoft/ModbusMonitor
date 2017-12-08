#include "dialogsetpid.h"
#include "ui_dialogsetpid.h"

DialogSetPid::DialogSetPid(const double initP, const double initI,
                           const double initD, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetPid)
{
    ui->setupUi(this);

    ui->spinP->setValue(initP);
    ui->spinI->setValue(initI);
    ui->spinD->setValue(initD);


}

DialogSetPid::~DialogSetPid()
{
    delete ui;
}


qreal DialogSetPid::getP() const
{
    return ui->spinP->value();
}

qreal DialogSetPid::getI() const
{
    return ui->spinI->value();
}

qreal DialogSetPid::getD() const
{
    return ui->spinD->value();
}

void DialogSetPid::on_btnSetPid_clicked()
{
    this->accept();
}

