#include "dialogarchive.h"
#include "ui_dialogarchive.h"

#include <QDebug>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QFileDialog>


DialogArchive::DialogArchive(QSqlTableModel *model, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogArchive)

{
    ui->setupUi(this);

    setupChart();
    setupModel();
    setupInterface();







}

DialogArchive::~DialogArchive()
{
    delete ui;
}

void DialogArchive::setupChart()
{

    _series = new QLineSeries();

    QDateTime now = QDateTime::currentDateTime();

    _chart = new QChart();
    _chart->addSeries(_series);
    _chart->setAnimationOptions(QChart::SeriesAnimations);

    axisX = new QDateTimeAxis(this);
    //axisX->setTickCount(CHART_TICK_COUNT);
    axisX->setFormat("yyyy-MM-dd<br>hh:mm:ss");
    axisX->setTitleText("Время");
    _chart->addAxis(axisX, Qt::AlignBottom);
    _series->attachAxis(axisX);

    axisX->setRange(QDateTime::currentDateTime().addDays(-1), QDateTime::currentDateTime().addDays(1));

    axisY = new QValueAxis(this);
    axisY->setLabelFormat("%+.1f");
    //axisY->setTickCount(11);
    axisY->setTitleText("Температура, &deg;С");
    _chart->addAxis(axisY, Qt::AlignLeft);
    _series->attachAxis(axisY);

    QChartView *chartView = new QChartView(_chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    ui->chartLayout->addWidget(chartView);



    axisX->setRange(now.addSecs(3600), now);
    axisY->setRange(0, 100);
}

void DialogArchive::setupInterface()
{
    ui->dtFrom->setDate(QDate::currentDate());
    ui->dtTo->setDate(QDate::currentDate().addDays(1));

    ui->tableView->setModel(_model);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->Export->setEnabled(false);

    this->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint);
}

void DialogArchive::setupModel()
{

    _model = new QSqlTableModel(this);
    _model->setTable("data");

    _model->setHeaderData(0, Qt::Horizontal, "Дата/время");
    _model->setHeaderData(1, Qt::Horizontal, "Время работы");
    _model->setHeaderData(2, Qt::Horizontal, "Время шага");
    _model->setHeaderData(3, Qt::Horizontal, "Заданная температура");
    _model->setHeaderData(4, Qt::Horizontal, "Текущая температура");






}

Ranges DialogArchive::getRanges(const QDateTime &dateFrom, const QDateTime &dateTo)
{
    QSqlQueryModel *rangesModel = new QSqlQueryModel(this);
//    QSqlQuery rangesQuery;
//    rangesQuery.prepare("select min(date_time) date_min, max(date_time)  date_max, min(currenttemp) temp_min, max(currenttemp) temp_max  "
//                        "from data "
//                        "WHERE date_time>=:dt_from and date_time<=:dt_to");
//    rangesQuery.bindValue(":dt_from", dateFrom.toString("yyyy-MM-dd hh:mm:ss"));
//    rangesQuery.bindValue(":dt_to", dateTo.toString("yyyy-MM-dd hh:mm:ss"));

//    rangesModel->setQuery(rangesQuery);

    rangesModel->setQuery(QString("select min(date_time) date_min, max(date_time)  date_max, min(currenttemp) temp_min, max(currenttemp) temp_max  "
                                  "from data "
                                  "WHERE date_time>='%1' and date_time<='%2'")
                          .arg(dateFrom.toString("yyyy-MM-dd hh:mm:ss"), dateTo.toString("yyyy-MM-dd hh:mm:ss")));


    qDebug() << "ROW COUNT" << rangesModel->rowCount() << rangesModel->query().lastQuery() << dateFrom.toString("yyyy-MM-dd hh:mm:ss");

    Ranges ranges;
    ranges.minDateTime = QDateTime::fromString(rangesModel->data(rangesModel->index(0, 0)).toString(), "yyyy-MM-dd hh:mm:ss");
    ranges.maxDateTime = QDateTime::fromString(rangesModel->data(rangesModel->index(0, 1)).toString(), "yyyy-MM-dd hh:mm:ss");
    ranges.minCurrentTemp = rangesModel->data(rangesModel->index(0, 2)).toFloat();
    ranges.maxCurrentTemp = rangesModel->data(rangesModel->index(0, 3)).toFloat();

    qDebug() << ranges.minDateTime;

    return ranges;

}

void DialogArchive::on_Search_clicked()
{

    QDateTime dateTimeFrom = ui->dtFrom->dateTime();
    QDateTime dateTimeTo = ui->dtTo->dateTime();


    _model->setFilter(QString("date_time>='%1' and date_time<='%2'")
                      .arg(
                          dateTimeFrom.toString("yyyy-MM-dd hh:mm:ss"),
                          dateTimeTo.toString("yyyy-MM-dd hh:mm:ss")
                          )
                      );
    _model->select();

    Ranges ranges = getRanges(dateTimeFrom, dateTimeTo);

   // qDebug() << "RANGES" << ranges.minDateTime << ranges.maxDateTime;

    axisX->setRange(ranges.minDateTime, ranges.maxDateTime);
    axisY->setRange(ranges.minCurrentTemp - 2, ranges.maxCurrentTemp + 2);

    _series->clear();
    ui->Export->setEnabled(false);

    while(_model->canFetchMore())
    _model->fetchMore();
    int rowCount =_model->rowCount();

    if (rowCount) {
        ui->Export->setEnabled(true);
    }



    for (int row = 0; row < rowCount; row++) {
        QString strDate = _model->data(_model->index(row, _model->fieldIndex("date_time"))).toString();
        QDateTime datetime = QDateTime::fromString(strDate, "yyyy-MM-dd hh:mm:ss");
        qreal value = _model->data(_model->index(row, _model->fieldIndex("currenttemp"))).toFloat();
        _series->append(datetime.toMSecsSinceEpoch(), value);
        qDebug() << row;

    }
    qDebug() << "CNT" << _model->rowCount();




}

void DialogArchive::on_Export_clicked()
{
    //QFileDialog csvFile;
    //csvFile.setFilter()
    //csvFile.exec();


    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "CSV File (*.csv)");
    QFile csvFile(fileName);


    QChar delimiter = ',';
    QString lineEnd = "\n";
    QStringList header = QStringList() << "date_time" << "uptime" << "steptime" << "targettemp" << "currenttemp";

    if (csvFile.open(QFile::WriteOnly|QFile::Truncate)) {
        QTextStream stream(&csvFile);
        stream << header.join(delimiter) << lineEnd;


        while(_model->canFetchMore())
        _model->fetchMore();
        int rowCount =_model->rowCount();

        for (int row = 0; row < rowCount; row++) {
            QString date_time = _model->data(_model->index(row, _model->fieldIndex("date_time"))).toString();
            QString uptime = _model->data(_model->index(row, _model->fieldIndex("uptime"))).toString();
            QString steptime = _model->data(_model->index(row, _model->fieldIndex("steptime"))).toString();
            QString targettemp = _model->data(_model->index(row, _model->fieldIndex("targettemp"))).toString();
            QString currenttemp = _model->data(_model->index(row, _model->fieldIndex("currenttemp"))).toString();

            QStringList line = QStringList()
                    << date_time
                    << uptime
                    << steptime
                    << targettemp
                    << currenttemp
                       ;

            stream << line.join(delimiter) << lineEnd;

        }



        csvFile.close();
    }


}
