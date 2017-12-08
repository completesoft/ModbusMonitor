#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDateTimeAxis>
#include <QMainWindow>


#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QSplineSeries>
#include <QValueAxis>
#include <QTimer>
#include <QSettings>

#include <QModbusClient>
#include "database.h"

#include "log.h"
#include "controller.h"
#include "controllerdata.h"


QT_CHARTS_USE_NAMESPACE

namespace Ui {
class MainWindow;
}


class RangeMinMax
{
public:
    qreal min;
    qreal max;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();




private slots:

    void onControllerPollComplete();

    void onControllerErrorOccured(const QString &errorText);

    void onControllerConnect();
    void onControllerConnecting();

    void onControllerStateChanged(Controller::ModbusState state);
    void onControllerReportReceived(Controller::Report report);

    void onControllerStatusChanged(Controller::Status status);


    void on_startButton_clicked();

    void timerShot();




    void on_btnSetParameters_clicked();

    void on_action_2_changed();

    void on_action_2_triggered();

    void seriesAdded(int index);



    void ping();

    void modbusStateChanged(QModbusDevice::State state);
    void modbusError(QModbusDevice::Error error);

    void modbusReplyFinished();
    void modbusReplyError(QModbusDevice::Error error);

    void sendRequestTemp();

    void on_action_3_triggered();

    void on_stopButton_clicked();

    void on_setPidButton_clicked();

    void onTimerEmulatorShot();


    void on_actionArchive_triggered();

private:
    Ui::MainWindow *ui;
//    QChartView *chartView;



    Controller *controller;

    QLineSeries *series;
    QLineSeries *series2;
//    QSplineSeries *series;
    QChart *chart;
//    QValueAxis *m_axis;
//    QValueAxis *axisY;
//    QDateTimeAxis *axisX;
    QTimer *timer;
    QTimer *pollTimer;
    int current_x;
    bool scroll;
    QSettings *settings;
    void loadSettings(const QString &iniFileName);
    QString getStrSetting(const QString &param);

    int CHART_INTERVAL_SEC;
    int CHART_X_RANGE_SEC;
    int MODBUS_PING_INTERVAL;
    int MODBUS_SERVER_ADDRESS;
    int MODBUS_MAIN_INTERVAL;

    qreal maxRangeFromSeries(QLineSeries *series);

    qreal minRangeFromSeries(QLineSeries *series);

    qreal maxRangeY;
    qreal minRangeY;

    QValueAxis *axisY;

    void setupChart();
    void setupMainTimer();

    QModbusClient *modbusClient;

    QTimer *timerPing;

    QModbusDataUnit pingData;

    void setupPing();

    void connectController();
    void mainTimerStop();

    bool loadDatabase(const QString &dbPath);
    Database *db;
    bool saveLineToDatabase(const QDateTime dateTime, const float temp);


    Log *logDlg;

    quint16 from8Bit(quint8 high, quint8 low);
    void setupController();


    QTimer *timerEmulator;

    qreal tempValue = 33.4;

    RangeMinMax getMinMaxRangeFromSeries(QLineSeries *ser, int countLastValues);

    void showTempOnChart(qreal temp);

    bool _chartScroll = true;

    QString timeSecToHMS(const quint32 sec);

    QString numberFormat(const qreal value);


};

#endif // MAINWINDOW_H
