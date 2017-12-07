#ifndef DIALOGSETPID_H
#define DIALOGSETPID_H

#include <QDialog>

namespace Ui {
class DialogSetPid;
}

class DialogSetPid : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSetPid(const double initP, const double initI,
                          const double initD, QWidget *parent = 0);
    ~DialogSetPid();


    int getP100();
    int getI100();
    int getD100();


private slots:
    void on_btnSetPid_clicked();

private:
    Ui::DialogSetPid *ui;
    int nint(double x);
};

#endif // DIALOGSETPID_H
