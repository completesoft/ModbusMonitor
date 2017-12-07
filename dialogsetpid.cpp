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

int DialogSetPid::getP100()
{
    return nint(ui->spinP->value()*100);
}

int DialogSetPid::getI100()
{
    return nint(ui->spinI->value()*100);
}

int DialogSetPid::getD100()
{
    return nint(ui->spinD->value()*100);
}

void DialogSetPid::on_btnSetPid_clicked()
{
    this->accept();
}

int DialogSetPid::nint(double x)
{
    return x < 0 ? (int)ceil(x) : (int)floor(x);
}
