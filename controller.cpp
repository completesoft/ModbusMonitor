#include "controller.h"
#include <QDebug>

Controller::Controller(
        const QString &ip_addr,
        const int ip_port,
        const int ip_modbus_addr,
        QObject *parent
        ) :
    QObject(parent),
    ipAddr(ip_addr),
    ipPort(ip_port),
    _ipModbusAddr(ip_modbus_addr),
    lastReply(nullptr),
    modbusClient(nullptr),
    _dataChanged(false),
    _controllerData(nullptr)
{


    _controllerData = new ControllerData(this);

    modbusClient = new QModbusTcpClient(this);

    modbusClient->setConnectionParameter(
                QModbusClient::NetworkAddressParameter,
                ipAddr
                );

    modbusClient->setConnectionParameter(
                QModbusClient::NetworkPortParameter,
                ipPort
                );

    connect(modbusClient, &QModbusTcpClient::stateChanged,
            this, &Controller::onStateChanged);

    connect(modbusClient, &QModbusTcpClient::errorOccurred,
            this, &Controller::onError);







}

void Controller::ipconnect()
{
    if (!modbusClient) {
        return;
    }

    modbusClient->connectDevice();

}

void Controller::start()
{


    _controllerData->setStart(1);


   // dataChangedOneValue(_regBase+_regStart, 1);

//    _dataToSend = _data;
//    _dataToSend[_regBase + _regStart] = 1;

//    _dataChanged = true;





}

void Controller::stop()
{


    _controllerData->setStop(1);
//    _dataToSend = _data;
//    _dataToSend[_regBase + _regStop] = 1;

//    _dataChanged = true;
}


QTime Controller::uptime()
{

}

QTime Controller::steptime()
{

}

void Controller::ping()
{
//    QModbusReply *reply = modbusClient->sendWriteRequest(pingDataUnit, ipModbusAddr);
//    connect(reply, &QModbusReply::finished, this, &Controller::onConrollerPingFinished);
//    qDebug() << "ping";
    readControllerStatus();


//    QModbusDataUnit du = QModbusDataUnit(QModbusDataUnit::HoldingRegisters, 22 ,1);
//    du.setValue(0, 1);
//    modbusClient->sendWriteRequest(du, 1);

}

void Controller::readDataResponse()
{

    auto *reply = static_cast<QModbusReply*>(sender());

    if (!reply) {
        return;
    }

    if (reply->error() == QModbusClient::NoError) {


//         _data = reply->result().values();

//         _data.clear();
//         foreach (quint16 value, reply->result().values()) {
//            _data.append(qint16(value));
//         }


        QVector<quint16> data = reply->result().values();
        _controllerData->fromModbusData(data);
        statusParsing(_controllerData->status());

        this->writeDataRequest();

        emit pollComplete();

//         this->setValues();
//         if (_dataChanged) {
//            this->writeDataRequest();
//         }




    }
    else {
        qDebug() << "controller status read error" << reply->errorString();
    }

    reply->deleteLater();


}

void Controller::writeDataResponse()
{


    auto *reply = static_cast<QModbusReply*>(sender());

    if (!reply) {
        return;
    }

    if (reply->error() == QModbusClient::NoError) {
        _controllerData->setChanged(false);

    }
    else {
        qDebug() << "controller status write error" << reply->errorString();
    }

    reply->deleteLater();

}

QList<float> Controller::temperatures()
{

}

void Controller::setParameters(const int targetTemp100, const quint32 targetStepTimeSec)
{

//    QVector<qint16> temp;
//    temp.append(targetTemp100);
//    temp.append(1);

//    dataChanged(_regBase+_regTargetTemp, 2, temp);


    qDebug() << targetStepTimeSec;

    _dataToSend = _data;

    _dataToSend[_regBase + _regSteptimeLo] = lo_from32Bit(targetStepTimeSec);
    _dataToSend[_regBase + _regSteptimeHi] = hi_from32Bit(targetStepTimeSec);


    _dataToSend[_regBase + _regTargetTemp] = targetTemp100;



    _dataChanged = true;


}

void Controller::setPID(const int kP100, const int kI100, const int kD100)
{

//    QVector<qint16> temp;
//    temp.append(kP100);
//    temp.append(kI100);
//    temp.append(kD100);
//    temp.append(1);

//    dataChanged(_regBase+_regPID_p, 4, temp);



    _dataToSend = _data;

    _dataToSend[_regBase + _regPID_p] = kP100;
    _dataToSend[_regBase + _regPID_i] = kI100;
    _dataToSend[_regBase + _regPID_d] = kD100;

    _dataChanged = true;




}

void Controller::setRegBase(const int address)
{
    _regBase = address;
}

void Controller::setRegCount(const int count)
{
    _regCount = count;
}

void Controller::setRegsTemp_1_8(const QList<int> offsetList)
{
    regsTemps = offsetList;
}

void Controller::setRegPing(const int offset)
{
    _regPing = offset;
}

void Controller::setPingInterval(const int interval)
{
    pingInterval = interval;

}

void Controller::setRegStart(const int offset)
{
    _regStart = offset;
}

void Controller::setRegStop(const int offset)
{
    _regStop = offset;
}

void Controller::setRegStatus(const int offset)
{
    _regStatus = offset;
}

void Controller::setRegTargetTemp(const int offset)
{
    _regTargetTemp = offset;
}

void Controller::setRegWriteTemp(const int offset)
{
    _regWriteTargetTemp = offset;
}

void Controller::setRegUptime(const int offset_lo, const int offset_hi)
{
    _regUptimeLo = offset_lo;
    _regUptimeHi = offset_hi;
}

void Controller::setRegSteptime(const int offset_lo, const int offset_hi)
{
    _regSteptimeLo = offset_lo;
    _regSteptimeHi = offset_hi;
}

void Controller::setRegsPID(
        const int offset_p,
        const int offset_i,
        const int offset_d,
        const int offset_write_pid)
{
    _regPID_p = offset_p;
    _regPID_i = offset_i;
    _regPID_d = offset_d;
    regPID_write = offset_write_pid;
}

ControllerData *Controller::controllerData() const
{
    return _controllerData;
}

void Controller::setControllerData(ControllerData *controllerData)
{
    _controllerData = controllerData;
}





int Controller::valueSteptimeHi() const
{
    return _valueSteptimeHi;
}

qint32 Controller::valueUptimeSec() const
{
    return from16Bit(this->_valueUptimeHi, this->_valueUptimeLo);
}

qint32 Controller::valueSteptimeSec() const
{
    return from16Bit(_valueSteptimeHi, _valueSteptimeLo);
}

int Controller::valueSteptimeLo() const
{
    return _valueSteptimeLo;
}

int Controller::valueUptimeHi() const
{
    return _valueUptimeHi;
}

int Controller::valueUptimeLo() const
{
    return _valueUptimeLo;
}

float Controller::valuePIDd() const
{
    return _valuePIDd;
}

float Controller::valuePIDi() const
{
    return _valuePIDi;
}

float Controller::valuePIDp() const
{
    return _valuePIDp;
}




float Controller::valueTargetTemp() const
{
    return _valueTargetTemp;
}

void Controller::poll()
{

    if (_controllerData->changed()) {
        this->writeDataRequest();
    }
    else {
        this->readDataRequest();
    }



}

int Controller::getIpPort() const
{
    return ipPort;
}

bool Controller::isConnected()
{
    if (modbusClient->state() == QModbusTcpClient::ConnectedState) {
        return true;
    }

    return false;
}

QString Controller::getIpAddr() const
{
    return ipAddr;
}


void Controller::sendData(const int address, const int data)
{

}

int Controller::readData(const int address)
{

}

void Controller::startReconnect()
{
    QTimer::singleShot(RECONNECT_INTERVAL, this, &Controller::ipconnect);

}

void Controller::readReport()
{


}

int Controller::readControllerStatus()
{
    this->readDataRequest();
}

void Controller::setupPing()
{
    pingTimer = new QTimer(this);
    pingTimer->setInterval(pingInterval);
    connect(pingTimer, &QTimer::timeout, this, &Controller::ping);

    pingDataUnit = QModbusDataUnit(QModbusDataUnit::HoldingRegisters,
                                   _regBase + _regPing, 1);

    pingDataUnit.setValue(0, 1);




}

void Controller::startPing()
{
    //pingTimer->start();
    ping();

}

qint32 Controller::from16Bit(qint16 high, qint16 low) const
{



    return (static_cast<qint32>(high) << 16) | low;
}

quint16 Controller::hi_from32Bit(quint32 input) const
{

    return static_cast<quint16>((input >> 16) & 0xffff);
}

quint16 Controller::lo_from32Bit(quint32 input) const
{



    quint16 *res_arr = (quint16*)&input;
    qDebug() << "LO HI" << res_arr[0] <<res_arr[1];


    return static_cast<quint16>(input & 0xffff);





}

QVector<float> Controller::valueTemperatures() const
{
    return _valueTemperatures;
}

QVector<qint16> Controller::getRawData() const
{
    return _data;
}



void Controller::readDataRequest()
{
    if (!modbusClient)
        return;

    if (modbusClient->state() == QModbusClient::UnconnectedState) {
        modbusClient->connectDevice();
        return;
    }

    int startAddress = _regBase;
    int countRegisters = _regCount;
    QModbusDataUnit readDataUnit(QModbusDataUnit::HoldingRegisters,
                                 startAddress, countRegisters);

    if (auto *reply = modbusClient->sendReadRequest(readDataUnit, _ipModbusAddr)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished,
                    this, &Controller::readDataResponse);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug() << "read error" << modbusClient->errorString();
    }


}

void Controller::writeDataRequest()
{
    if (!modbusClient)
        return;


    _controllerData->setPing(1); //ping modify


//    if (_dataChanged) {
//        _dataChanged = false;

//    }
//    else {
//      _dataToSend = _data;
//    }

//    _dataToSend[_regBase + _regPing] = 1; //ping modify



//    //qint16 vector to quint16 vector
//    QVector<quint16> writeData;
//    foreach (qint16 value, _dataToSend) {
//        writeData.append(quint16(value));
//    }


    QModbusDataUnit writeDataUnit(QModbusDataUnit::HoldingRegisters,
                                 _regBase, _controllerData->toModbusData());


    if (auto *reply = modbusClient->sendWriteRequest(writeDataUnit, _ipModbusAddr)) {
        if (!reply->isFinished())
            connect(reply, &QModbusReply::finished,
                    this, &Controller::writeDataResponse);
        else
            delete reply; // broadcast replies return immediately
    } else {
        qDebug() << "write error" << modbusClient->errorString();
    }



}

void Controller::setValues()
{

    QVector<qint16> int100Temperatures = _data.mid(_regBase, TEMP_COUNT);



    _valueTemperatures = fromInt100ToFloatQVector(int100Temperatures);

    _valueTargetTemp = _data[_regBase + _regTargetTemp]/100.0;

    _valuePIDp = _data[_regBase + _regPID_p]/100.0;
    _valuePIDi = _data[_regBase + _regPID_i]/100.0;
    _valuePIDd = _data[_regBase + _regPID_d]/100.0;

    _valueUptimeLo = _data[_regBase + _regUptimeLo];
    _valueUptimeHi = _data[_regBase + _regUptimeHi];

    _valueSteptimeLo = _data[_regBase + _regSteptimeLo];
    _valueSteptimeHi = _data[_regBase + _regSteptimeHi];


    statusParsing(_data[_regBase + _regStatus]);





    emit pollComplete();

}

QVector<float> Controller::fromInt100ToFloatQVector(const QVector<qint16> vector) const
{
    QVector<float> result;

    for (int i=0; i<vector.count(); i++) {
        result.append(vector[i]/100.0);
    }

    return result;

}

void Controller::dataChanged(const quint16 from, const quint16 count, const QVector<qint16> changedData)
{
    _dataChanged = true;
    _dataChangedFrom = from;
    _dataChangedCount = count;
    _dataChangedNewData.clear();
    _dataChangedNewData = changedData;

}

void Controller::dataChangedOneValue(const quint16 addr, const qint16 value)
{
    _dataChanged = true;
    _dataChangedFrom = addr;
    _dataChangedCount = 1;
    _dataChangedNewData.clear();
    _dataChangedNewData.append(1);
}

QVector<qint16> Controller::dataModify(
        const QVector<qint16> sourceData,
        const quint16 from,
        const quint16 count,
        const QVector<qint16> changedData
        ) const
{
    QVector<qint16> result = sourceData;

    for (int i=from; i<(count+from); i++){
        result[i] = changedData.at(i-from);
    }

    return result;
}

void Controller::statusParsing(quint16 statusCode)
{

        if (statusCode == 0) {
            _status = Controller::StatusStopped;
        }
        else if (statusCode >0 && statusCode <10000) {
            _status = Controller::StatusError;
            _errorCode = statusCode;
        }
        else if (statusCode == 10000) {
            _status = Controller::StatusStarted;
        }
        else if (statusCode >10000) {
            _status = Controller::StatusError;
            emit modbusErrorOccured(QString("Получено недопустимое значение статуса (%1)")
                                    .arg(statusCode));
        }

        if (_statusCode != statusCode) {
            _statusCode = statusCode;
            emit statusChanged(_status);
        }
}



int Controller::errorCode() const
{
    return _errorCode;
}

void Controller::onStateChanged(QModbusDevice::State state)
{

    switch (state) {

    case QModbusDevice::ConnectingState:
        emit modbusStateChanged(Controller::Connecting);
        break;

    case QModbusDevice::ConnectedState:
        emit modbusStateChanged(Controller::Connected);
        break;

    case QModbusDevice::UnconnectedState:
        emit modbusStateChanged(Controller::Disconnected);
        break;


    default:
        break;
    }

}

void Controller::onError(QModbusDevice::Error error)
{

    if (error == QModbusDevice::ConnectionError) {
        emit modbusErrorOccured("Ошибка подключения. " + modbusClient->errorString());

        startReconnect();
    }
    else {
        emit modbusErrorOccured("Ошибка");
        qDebug() << modbusClient->errorString();

    }



}

void Controller::onControllerStatusReply()
{

    auto *reply = static_cast<QModbusReply*>(sender());

    if (!reply) {
        return;
    }

    if (reply->error() == QModbusClient::NoError) {
        qDebug() << "status read " << reply->result().value(0);
    }
    else {
        qDebug() << "controller status read error" << reply->errorString();
    }

    reply->deleteLater();

    //    QModbusReply *reply = static_cast<QModbusReply*>(sender());
//    int readStatusCode = reply->result().value(0);

//    qDebug () << readStatusCode;

//    if (readStatusCode == 0) {
//        m_status = Controller::StatusStopped;
//    }
//    else if (readStatusCode >0 && readStatusCode <10000) {
//        m_status = Controller::StatusError;
//        m_errorCode = readStatusCode;
//    }
//    else if (readStatusCode == 10000) {
//        m_status = Controller::StatusStarted;
//    }
//    else if (readStatusCode >10000) {
//        m_status = Controller::StatusError;
//        emit modbusErrorOccured(QString("Получено недопустимое значение статуса (%1)")
//                                .arg(readStatusCode));
//    }

//    if (m_statusCode != readStatusCode) {
//        m_statusCode = readStatusCode;
//        emit statusChanged(m_status);
//    }

}

void Controller::onControllerPollFinished()
{
    QModbusReply *reply = static_cast<QModbusReply*>(sender());

    ControllerData controllerData;

    QVector<quint16> data = reply->result().values();
    controllerData.fromModbusData(data);
    qDebug() << "NEW" << controllerData.temp1() << controllerData.realTemp1();


//    _valueTargetTemp = reply->result().value(_regTargetTemp);
//    _valuePIDp = reply->result().value(_regPID_p);
//    _valuePIDi = reply->result().value(_regPID_i);
//    _valuePIDd = reply->result().value(_regPID_d);

//    _valueUptimeLo = reply->result().value(_regUptimeLo);
//    _valueUptimeHi = reply->result().value(_regUptimeHi);
//    _valueSteptimeLo = reply->result().value(_regSteptimeLo);
//    _valueSteptimeHi = reply->result().value(_regSteptimeHi);




    emit pollComplete();


}

void Controller::onConrollerPingFinished()
{
    qDebug() << "ping finished";
    QModbusReply *reply = static_cast<QModbusReply*>(sender());
    qDebug() << "rep" << reply->errorString();

}
