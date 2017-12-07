#include "dialogsettemp.h"
#include "ui_dialogsettemp.h"

DialogSetTemp::DialogSetTemp(const double initialValue, const double targetStepTimeHours, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSetTemp)
{
    ui->setupUi(this);
    ui->doubleSpinBox->setValue(initialValue);
    ui->doubleSpinBox_2->setValue(targetStepTimeHours);
}

DialogSetTemp::~DialogSetTemp()
{
    delete ui;
}

int DialogSetTemp::getTemperature100()
{
    return nint(ui->doubleSpinBox->value()*100);
}

quint32 DialogSetTemp::getTargetStepTimeSec()
{
    return nint(ui->doubleSpinBox_2->value()*60*60);
}

void DialogSetTemp::on_pushButton_clicked()
{
    this->accept();
}

int DialogSetTemp::nint(double x)
{
    return x < 0 ? (int)ceil(x) : (int)floor(x);
}


