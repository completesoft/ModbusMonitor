#ifndef LOG_H
#define LOG_H

#include <QDialog>

namespace Ui {
class Log;
}

class Log : public QDialog
{
    Q_OBJECT

public:
    explicit Log(QWidget *parent = 0);
    ~Log();

public slots:
    void addMessage(const QString &message);
    void addError(const QString &message);
    void rotate(const QString &fileName);
    void addVectorData(QVector<qint16> data);


private slots:


private:
    Ui::Log *ui;
    void saveToFile(const QString &fileName);
};

#endif // LOG_H
