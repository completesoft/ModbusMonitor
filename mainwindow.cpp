#include "mainwindow.h"
#include "ui_mainwindow.h"




#include <QDebug>
#include <QDateTime>
#include <QCategoryAxis>
#include <QMessageBox>
#include <QTextCodec>
#include <QModbusTcpClient>

#include "dialogsettemp.h"
#include "dialogexport.h"
#include "dialogsetpid.h"
#include "dialogarchive.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    modbusClient(nullptr)
{
    ui->setupUi(this);
    //this->resize(1300, 760);
    this->setWindowState(Qt::WindowMaximized);

    logDlg = new Log(this);

    loadSettings("mbmon.ini");

    setupController();

    pollTimer = new QTimer(this);
    pollTimer->setInterval(settings->value("Timers/poll_timer", 1000).toInt());
    connect(pollTimer, &QTimer::timeout, controller, &Controller::poll);


    setupChart();
    loadDatabase("db/base.db");



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onControllerPollComplete()
{


//    qDebug() << controller->getControllerData()->realTemp1();

//    int indexCurrentTemperature = settings->value("Chart/current_temperature_index", 0).toInt() - 1;

    ui->lblTargetTemp->setText(numberFormat(controller->controllerData()->realTargetTemp()));
    ui->lblCurrentTemp->setText(numberFormat(controller->controllerData()->realTemp1()));

    showTempOnChart(controller->controllerData()->realTemp1());

   // onTimerEmulatorShot();

    ui->lblP->setText(numberFormat(controller->controllerData()->realPIDp()));
    ui->lblI->setText(numberFormat(controller->controllerData()->realPIDi()));
    ui->lblD->setText(numberFormat(controller->controllerData()->realPIDd()));

    ui->lblUptime->setText(timeSecToHMS(controller->controllerData()->fullUptimeSec()));

    ui->lblSteptime->setText(timeSecToHMS(controller->controllerData()->fullSteptimeSec()));

//    logDlg->addVectorData(controller->getRawData());

    db->saveValues(
                QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"),
                timeSecToHMS(controller->controllerData()->fullUptimeSec()),
                timeSecToHMS(controller->controllerData()->fullSteptimeSec()),
                numberFormat(controller->controllerData()->realTargetTemp()),
                numberFormat(controller->controllerData()->realTemp1())
                );



}

void MainWindow::onControllerErrorOccured(const QString &errorText)
{
    logDlg->addError(errorText);

}

void MainWindow::onControllerConnect()
{
    //logDlg->addMessage("Контроллер подключен.");
}

void MainWindow::onControllerConnecting()
{
    //logDlg->addMessage("Подключение...");
}

void MainWindow::onControllerStateChanged(Controller::ModbusState state)
{

    switch (state) {

    case Controller::Connecting:
        logDlg->addMessage("Подключение к контроллеру...");
        break;

    case Controller::Connected:
        logDlg->addMessage("Подключение установлено.");
        pollTimer->start();
        controller->poll();
        ui->labelError->setText("");

        break;

    case Controller::Disconnected:
        logDlg->addMessage("Подключение разорвано.");
        ui->labelError->setText("Соединение разорвано");
        break;

    default:
        break;
    }


}

void MainWindow::onControllerReportReceived(Controller::Report report)
{
    switch (report) {
    case Controller::ReportStarted:
        logDlg->addMessage("Старт выполнен.");
        break;
    default:
        break;
    }

}

void MainWindow::onControllerStatusChanged(Controller::Status status)
{

    bool startEnable = (status == Controller::StatusStopped);

    ui->startButton->setEnabled(startEnable);
    ui->stopButton->setEnabled(!startEnable);
    ui->btnSetParameters->setEnabled(startEnable);

    switch (status) {
    case Controller::StatusStarted:
        logDlg->addMessage("Контроллер запущен");
        ui->labelError->setText("");
        break;
    case Controller::StatusStopped:
        logDlg->addMessage("Контроллер остановлен");
        ui->labelError->setText("");
        break;
    case Controller::StatusError:
        logDlg->addMessage(QString("Ошибка контроллера, код ошибки - (%1)")
                           .arg(controller->errorCode()));
//        QMessageBox::critical(
//                    this,
//                    "ОШИБКА",
//                    settings->value(QString("Errors/%1").arg(controller->errorCode()),
//                                    QString("Ошибка №%1").arg(controller->errorCode()))
//                    .toString()
//                    );

        ui->labelError->setText(settings->value(QString("Errors/%1").arg(controller->errorCode()),
                                            QString("Ошибка №%1").arg(controller->errorCode())).toString());

        qDebug() << "ErrorString" << QString("ErrorDict/%1").arg(controller->errorCode());

        break;
    default:
        break;
    }

}

void MainWindow::on_startButton_clicked()
{

    logDlg->addMessage("Старт...");
    controller->start();


}

void MainWindow::timerShot()
{


    QDateTime momentInTime;

    QDateTime now = QDateTime::currentDateTime();

    QDate nowDate = now.date();
    QTime nowTime = now.time();

    momentInTime.setDate(nowDate);

    momentInTime.setTime(nowTime);

    qreal temperature = log(current_x++)*15+ (qrand() % 5);
    qreal temperature2 = log(current_x)*20+ (qrand() % 15);
    ui->lblCurrentTemp->setText(QString::number(temperature, 'f', 2));

//    qreal temperature = 45.88;

    series->append(momentInTime.toMSecsSinceEpoch(), temperature);
    series2->append(momentInTime.toMSecsSinceEpoch(), temperature2);




//    ui->lcdNumber->display(temperature);





    if (current_x >= CHART_X_RANGE_SEC/CHART_INTERVAL_SEC) {

        scroll = true;
    }

    if (scroll) {
        chart->scroll(chart->plotArea().width()/CHART_X_RANGE_SEC*CHART_INTERVAL_SEC, 0);
        series->removePoints(0, 1);
    }


}


void MainWindow::loadSettings(const QString &iniFileName)
{
    logDlg->addMessage("Чтение настроек...");
    QTextCodec *codec = QTextCodec::codecForName("Windows-1251");
    settings = new QSettings(iniFileName, QSettings::IniFormat);
    settings->setIniCodec(codec);

    if (settings->value("Debug/debug_log_on_startup", 0).toBool()) {
        logDlg->show();
    }




//    QMessageBox::critical(this,"TEST", settings->value("ErrorDict/55", 999).toString());






}


qreal MainWindow::maxRangeFromSeries(QLineSeries *series)
{


}

void MainWindow::setupChart()
{

    scroll = false;

    CHART_INTERVAL_SEC = 1;
    CHART_X_RANGE_SEC = settings->value("Chart/chart_time_width", 60).toInt();
    int CHART_X_RANGE_DIV = settings->value("Chart/chart_time_div", 10).toInt();
    int CHART_TICK_COUNT = CHART_X_RANGE_SEC/(CHART_X_RANGE_DIV-1) + 1;

    maxRangeY = 34;
    minRangeY = 33;

    series = new QLineSeries();

    connect(series, SIGNAL(pointAdded(int)), this, SLOT(seriesAdded(int)));

//    series->setColor("#FF1122");
    series->setName(settings->value("Chart/legend_temp1", "Температура").toString());
//    series2 = new QLineSeries();

//    connect(series2, SIGNAL(pointAdded(int)), this, SLOT(seriesAdded(int)));
//    series = new QSplineSeries();
    QDateTime momentInTime;

    QDateTime now = QDateTime::currentDateTime();



//    QDate nowDate = now.date();
//    QTime nowTime = now.time();

//    momentInTime.setDate(nowDate);

//    momentInTime.setTime(nowTime);
//    series->append(momentInTime.toMSecsSinceEpoch(), 50);


    chart = new QChart();
    chart->addSeries(series);
//    chart->addSeries(series2);

//    chart->legend()->hide();
//    chart->setAnimationOptions(QChart::SeriesAnimations);

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setTickCount(CHART_TICK_COUNT);
    axisX->setFormat("hh:mm:ss");
    axisX->setTitleText("Время");
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);
//    series2->attachAxis(axisX);




    axisY = new QValueAxis;
    axisY->setLabelFormat("%.1f");
    axisY->setTickCount(11);
    axisY->setTitleText("Температура, &deg;С");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);
//    series2->attachAxis(axisY);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);


    ui->horizontalLayout->addWidget(chartView);



    axisX->setRange(now.addSecs(-CHART_X_RANGE_SEC), now);
    axisY->setRange(minRangeY, maxRangeY);




    current_x = 1;



    if (settings->value("Debug/debug_emulator", 0).toBool()) {
        timerEmulator = new QTimer(this);
        timerEmulator->setInterval(1000);
        connect(timerEmulator, &QTimer::timeout, this, &MainWindow::onTimerEmulatorShot);
        timerEmulator->start();

    }

////    series = new QSplineSeries();

//    current_x = 360;

////    m_axis = new QValueAxis(this);
//    m_axis = new QDateTimeAxis(this);
//    m_axis->setFormat("hh:mm");
//    m_axis->setTitleText("Время");
//    chart = new QChart();
//    chart->addSeries(series);
//    chart->legend()->hide();
//    chart->createDefaultAxes();
//    chart->setAxisX(m_axis, series);
//    m_axis->setTickCount(11);
////    chart->setAnimationOptions(QChart::AllAnimations);

//    chartView = new QChartView(chart);
//    chartView->setRenderHint(QPainter::Antialiasing);


//    ui->horizontalLayout->addWidget(chartView);

    //    chart->axisY()->setRange(-200,+200);
}

void MainWindow::setupMainTimer()
{
    timer = new QTimer(this);
    timer->setInterval(MODBUS_MAIN_INTERVAL);
    connect(timer, SIGNAL(timeout()), this, SLOT(sendRequestTemp()));

}

void MainWindow::setupPing()
{
    pingData = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, settings->value("Registers/ping", 22).toInt(), 1);
    pingData.setValue(0, 0);

    timerPing = new QTimer(this);
    timerPing->setInterval(MODBUS_PING_INTERVAL);
    connect(timerPing, &QTimer::timeout,this, &MainWindow::ping);

}

void MainWindow::connectController()
{
    ui->statusBar->showMessage("Подключение...");

    modbusClient = new QModbusTcpClient(this);

    connect(modbusClient, &QModbusClient::stateChanged, this, &MainWindow::modbusStateChanged);
    connect(modbusClient, &QModbusClient::errorOccurred, this, &MainWindow::modbusError);


    modbusClient->setConnectionParameter(QModbusClient::NetworkAddressParameter, settings->value("Network/controller_addr", "127.0.0.1").toString());
    modbusClient->setConnectionParameter(QModbusClient::NetworkPortParameter, settings->value("Network/controller_port", "502").toString());




    if (!modbusClient->connectDevice()) {

    }

}

void MainWindow::mainTimerStop()
{
    timer->stop();

}

bool MainWindow::loadDatabase(const QString &dbPath)
{

    db = new Database(dbPath, this);
    if (db->openError()){

        return false;

    }

    return true;

}

bool MainWindow::saveLineToDatabase(const QDateTime dateTime, const float temp)
{
    QSqlTableModel *model = new QSqlTableModel(this);
    model->setTable("data");
    //model->select();

    QSqlRecord record = model->record();
    record.setValue("date_time", dateTime);
    record.setValue("temp01", QString::number(temp, 'f', 2));
    return model->insertRecord(-1, record);


}

void MainWindow::setupController()
{
    controller = new Controller(
                settings->value("Network/controller_ip_addr", "127.0.0.1").toString(),
                settings->value("Network/controller_ip_port", 502).toInt(),
                settings->value("Network/controller_modbus_addr", 1).toInt()
                );

    controller->setRegBase(
                settings->value("Registers/base_address", 0).toInt()
                );

    controller->setRegCount(
                settings->value("Registers/register_count", 22).toInt()
                );

    controller->setRegStart(
                settings->value("Registers/start", 14).toInt()
                );

    controller->setRegStop(
                settings->value("Registers/stop", 15).toInt()
                );

    controller->setRegPing(
                settings->value("Registers/ping", 22).toInt()
                );

    controller->setPingInterval(
                settings->value("Timers/ping_interval", 1000).toInt()
                );

    controller->setRegStatus(
                settings->value("Registers/status", 16).toInt()
                );

    controller->setRegTargetTemp(
                settings->value("Registers/target_temp", 12).toInt()
                );

    controller->setRegWriteTemp(
                settings->value("Registers/write_temp", 13).toInt()
                );

    controller->setRegsPID(
                settings->value("Registers/k_p", 8).toInt(),
                settings->value("Registers/k_i", 9).toInt(),
                settings->value("Registers/k_d", 10).toInt(),
                settings->value("Registers/write_pid", 11).toInt()
                );

    controller->setRegUptime(
                settings->value("Registers/uptime_lo", 18).toInt(),
                settings->value("Registers/uptime_hi", 19).toInt()
                );

    controller->setRegSteptime(
                settings->value("Registers/steptime_lo", 20).toInt(),
                settings->value("Registers/steptime_hi", 21).toInt()
                );


    connect(controller, &Controller::modbusErrorOccured, this, &MainWindow::onControllerErrorOccured);
    connect(controller, &Controller::modbusStateChanged, this, &MainWindow::onControllerStateChanged);
    connect(controller, &Controller::statusChanged, this, &MainWindow::onControllerStatusChanged);
    connect(controller, &Controller::pollComplete, this, &MainWindow::onControllerPollComplete);

    controller->ipconnect();
}

RangeMinMax MainWindow::getMinMaxRangeFromSeries(QLineSeries *ser, int countLastValues)
{




    int from = ser->count() - countLastValues;
    if (from<1) {
        from=1;
    }

    qreal max = ser->at(from-1).y();
    qreal min = ser->at(from-1).y();



    for (int i=from; i<ser->count(); i++) {
        qreal currentValue = ser->at(i).y();
        if (currentValue > max) {
            max = currentValue;
        }

        if (currentValue < min) {
            min = currentValue;
        }
    }






    RangeMinMax rangeMinMax;
    rangeMinMax.min = min;
    rangeMinMax.max = max;

    return rangeMinMax;
}

void MainWindow::showTempOnChart(qreal temp)
{
    QDateTime momentInTime;

    QDateTime now = QDateTime::currentDateTime();

    QDate nowDate = now.date();
    QTime nowTime = now.time();

    momentInTime.setDate(nowDate);

    momentInTime.setTime(nowTime);


    series->append(momentInTime.toMSecsSinceEpoch(), temp);
    qDebug() << "count" <<series->count();
    if (series->count() > CHART_X_RANGE_SEC*CHART_INTERVAL_SEC) {
        series->removePoints(0, 1);
        qDebug() << "remove";
    }

    RangeMinMax rangeMinMax = getMinMaxRangeFromSeries(series, CHART_X_RANGE_SEC*CHART_INTERVAL_SEC);


    if (_chartScroll) {
        chart->scroll(chart->plotArea().width()/CHART_X_RANGE_SEC*CHART_INTERVAL_SEC, 0);

    }

    qreal diff = rangeMinMax.max - rangeMinMax.min;
    qreal div_1 = settings->value("Chart/div_1", .2).toReal();
    qreal div_10 = settings->value("Chart/div_10", 1).toReal();
    qreal div_100 = settings->value("Chart/div_100", 10).toReal();

    if (diff <= 1) {
        axisY->setLabelFormat("%+.2f");
        qreal minY = qRound(rangeMinMax.min - 0.6);
        qreal maxY = qRound(rangeMinMax.max + 0.6);
        axisY->setTickCount(qRound(abs(maxY-minY)/div_1)+1);
        //axisY->setTickCount(16);

        axisY->setRange(minY, maxY);
    }
    else if ((diff > 1) && (diff <=10)) {

        axisY->setLabelFormat("%+.1f");
        qreal minY = qRound(rangeMinMax.min - 2);
        qreal maxY = qRound(rangeMinMax.max + 2);
        axisY->setTickCount(qRound(abs(maxY-minY)/div_10)+1);
        axisY->setRange(minY, maxY);
    }
    else if (diff > 10) {

        axisY->setLabelFormat("%+.0f");
        qreal minY = qRound(rangeMinMax.min / 10)*10 - 10;
        qreal maxY = qRound(rangeMinMax.max / 10)*10 + 10;
        axisY->setTickCount(qRound(abs(maxY-minY)/div_100)+1);
        axisY->setRange(minY, maxY);
    }




}

QString MainWindow::timeSecToHMS(const quint32 sec)
{
    quint32 minutes = sec / 60;
    quint32 seconds = sec % 60;
    quint32 hours = minutes / 60;



    minutes = minutes % 60;

    return QString("%1:%2:%3")
            .arg(hours, 2, 10, QLatin1Char('0'))
            .arg(minutes, 2, 10, QLatin1Char('0'))
            .arg(seconds, 2, 10, QLatin1Char('0'));


}

QString MainWindow::numberFormat(const qreal value)
{
    return QString::number(value, 'f', 2);

}

void MainWindow::sendRequestTemp()
{

        QModbusDataUnit data;
        data.setRegisterType(QModbusDataUnit::HoldingRegisters);
        data.setStartAddress(0);
        data.setValueCount(1);

        QModbusReply *reply = new QModbusReply(QModbusReply::Common, 1);


        reply = modbusClient->sendReadRequest(data, MODBUS_SERVER_ADDRESS);

        connect(reply, &QModbusReply::finished, this, &MainWindow::modbusReplyFinished);
      //  connect(reply, &QModbusReply::errorOccurred, this, &MainWindow::modbusReplyError);
}



void MainWindow::on_btnSetParameters_clicked()
{

    DialogSetTemp dlg(ui->lblTargetTemp->text().toDouble(), controller->controllerData()->fullSteptimeSec()/3600.0);
    if (dlg.exec()) {
        controller->controllerData()->setFullSteptime(dlg.getTargetStepTimeSec());
        controller->controllerData()->setRealTargetTemp(dlg.getTargetTemp());
    }
}

void MainWindow::on_action_2_changed()
{

}

void MainWindow::on_action_2_triggered()
{
    DialogExport dlgExport;
    dlgExport.exec();
}

void MainWindow::seriesAdded(int index)
{
//    qreal currentY = static_cast<QLineSeries*>(sender())->at(index).y();
//    if (maxRangeY <= currentY ) {
//        maxRangeY = (qRound(currentY) % 10 +1)*10 ;
//        axisY->setMax(maxRangeY);
//    }

}



void MainWindow::ping()
{
    modbusClient->sendWriteRequest(pingData, MODBUS_SERVER_ADDRESS);
}

void MainWindow::modbusStateChanged(QModbusDevice::State state)
{
    if (state == QModbusDevice::ConnectedState) {
        ui->statusBar->showMessage("Контроллер подключен", 5000);
        timerPing->start();
    }

}

void MainWindow::modbusError(QModbusDevice::Error error)
{

    if (error == QModbusDevice::ConnectionError){


       int result = QMessageBox::critical(this, "Ошибка подключения", QString("Ошибка подключения: %1<br>Сетевой адрес: %2<br>Порт: %3<br><br>Продолжить?").arg(modbusClient->errorString())
                              .arg(settings->value("Network/controller_addr", "127.0.0.1").toString())
                              .arg(settings->value("Network/controller_port", "502").toString()),

                              QMessageBox::Yes | QMessageBox::No, QMessageBox::No);

       if (result == QMessageBox::No) this->close();


    }





}

void MainWindow::modbusReplyFinished()
{

    qreal temp = static_cast<QModbusReply*>(sender())->result().value(0)/100.0;




    QDateTime momentInTime;

    QDateTime now = QDateTime::currentDateTime();

    QDate nowDate = now.date();
    QTime nowTime = now.time();

    momentInTime.setDate(nowDate);

    momentInTime.setTime(nowTime);

    ui->lblCurrentTemp->setText(QString::number(temp, 'f', 2));

//    qreal temperature = 45.88;

    series->append(momentInTime.toMSecsSinceEpoch(), temp);




}

void MainWindow::modbusReplyError(QModbusDevice::Error error)
{


}

void MainWindow::on_action_3_triggered()
{
    logDlg->show();
}

inline quint16 from8Bit(quint8 high, quint8 low)
{
    return (static_cast<quint16>(high) << 8) | low;
}

void MainWindow::on_stopButton_clicked()
{
    logDlg->addMessage("Стоп...");
    controller->stop();
}

void MainWindow::on_setPidButton_clicked()
{
    DialogSetPid dlg(ui->lblP->text().toFloat(), ui->lblI->text().toFloat(),
                     ui->lblD->text().toFloat());
    if (dlg.exec()) {

        controller->controllerData()->setRealPIDp(dlg.getP());
        controller->controllerData()->setRealPIDi(dlg.getI());
        controller->controllerData()->setRealPIDd(dlg.getD());
        controller->controllerData()->setWritePid(1);


//        controller->setPID(dlg.getP100(), dlg.getI100(), dlg.getD100());
    }
}

void MainWindow::onTimerEmulatorShot()
{
    qreal randomTemp;
    randomTemp = (qrand() % 200)/10.0;

    if (series->count()  > 20) {
        randomTemp = (qrand() % 20)/10.0;
    }

    if (series->count()  > 30) {
        randomTemp = (qrand() % 9)/10.0;
    }

    showTempOnChart(randomTemp);
}






void MainWindow::on_actionArchive_triggered()
{
    DialogArchive dlg(db->getModel());
    dlg.exec();
}
