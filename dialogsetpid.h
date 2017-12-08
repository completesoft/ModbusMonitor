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


    qreal getP() const;
    qreal getI() const;
    qreal getD() const;



private slots:
    void on_btnSetPid_clicked();

private:
    Ui::DialogSetPid *ui;
};

#endif // DIALOGSETPID_H
