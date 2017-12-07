#ifndef DIALOGARCHIVE_H
#define DIALOGARCHIVE_H

#include <QDialog>
#include <QChart>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QValueAxis>
#include <QChartView>
#include <QSqlTableModel>
#include <QDateTime>

QT_CHARTS_USE_NAMESPACE

namespace Ui {
class DialogArchive;
}

class Ranges {

public:

    qreal minCurrentTemp;
    qreal maxCurrentTemp;
    QDateTime minDateTime;
    QDateTime maxDateTime;

};

class DialogArchive : public QDialog
{
    Q_OBJECT

public:
    explicit DialogArchive(QSqlTableModel *model, QWidget *parent = 0);
    ~DialogArchive();

private slots:
    void on_Search_clicked();

    void on_Export_clicked();

private:
    Ui::DialogArchive *ui;

    void setupChart();
    void setupInterface();
    void setupModel();


    Ranges getRanges(const QDateTime &dateFrom, const QDateTime &dateTo);

    QLineSeries *_series;
    QChart *_chart;
    QSqlTableModel *_model;


    QDateTimeAxis *axisX;
    QValueAxis *axisY;
};

#endif // DIALOGARCHIVE_H
