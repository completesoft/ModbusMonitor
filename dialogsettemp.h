#ifndef DIALOGSETTEMP_H
#define DIALOGSETTEMP_H

#include <QDialog>

namespace Ui {
class DialogSetTemp;
}

class DialogSetTemp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetTemp(const double initialValue, const double targetStepTimeHours, QWidget *parent = 0);
    ~DialogSetTemp();

    int getTemperature100();
    quint32 getTargetStepTimeSec();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogSetTemp *ui;

    int nint(double x);
};

#endif // DIALOGSETTEMP_H
