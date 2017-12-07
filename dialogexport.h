#ifndef DIALOGEXPORT_H
#define DIALOGEXPORT_H

#include <QDialog>

namespace Ui {
class DialogExport;
}

class DialogExport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogExport(QWidget *parent = 0);
    ~DialogExport();

private:
    Ui::DialogExport *ui;
};

#endif // DIALOGEXPORT_H
