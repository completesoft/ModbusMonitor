#ifndef CONTROLLER_H
#define CONTROLLER_H

#define TEMP_COUNT 8

#include <QObject>
#include <QTime>
#include <QModbusTcpClient>
#include <QModbusDataUnit>
#include <QTimer>

#include "controllerdata.h"

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(
            const QString &ip_addr,
            const int ip_port,
            const int ip_modbus_addr,
            QObject *parent = nullptr
            );

    QString getIpAddr() const;
    int getIpPort() const;
    bool isConnected();
    enum ModbusState {Connecting, Connected, Disconnected, Error};
    enum Report {ReportStarted, ReportStopped, ReportPidSet, ReportTempSet};
    enum Status {StatusStarted, StatusStopped, StatusError};



    int errorCode() const;

    float valueTargetTemp() const;

    float valuePIDp() const;

    float valuePIDi() const;

    float valuePIDd() const;



    int valueUptimeLo() const;

    int valueUptimeHi() const;

    int valueSteptimeLo() const;

    int valueSteptimeHi() const;

    qint32 valueUptimeSec() const;

    qint32 valueSteptimeSec() const;

    int readControllerStatus();

    QVector<float> valueTemperatures() const;

    QVector<qint16> getRawData() const;


    ControllerData *controllerData() const;
    void setControllerData(ControllerData *controllerData);

signals:
    void modbusErrorOccured(const QString &error);
    void modbusStateChanged(Controller::ModbusState state);
    void reportReceived(Controller::Report report);
    void statusChanged(Status _status);
    void pollComplete();


public slots:

    void poll();
    void ipconnect();
    void start();
    void stop();
    QTime uptime();
    QTime steptime();
    QList<float> temperatures();
    void setParameters(const int targetTemp100, const quint32 targetStepTimeSec);
    void setPID(const int kP100, const int kI100, const int kD100);

    void setRegBase(const int address);
    void setRegCount(const int count);
    void setRegsTemp_1_8(const QList<int> offsetList);
    void setRegPing(const int offset);
    void setPingInterval(const int interval);
    void setRegStart(const int offset);
    void setRegStop(const int offset);
    void setRegStatus(const int offset);
    void setRegTargetTemp(const int offset);
    void setRegWriteTemp(const int offset);
    void setRegUptime(const int offset_lo, const int offset_hi);
    void setRegSteptime(const int offset_lo, const int offset_hi);


    void setRegsPID(
            const int offset_p,
            const int offset_i,
            const int offset_d,
            const int offset_write_pid
            );

private:


    ControllerData *_controllerData;

    int _regBase;
    int _regCount;
    QList<int> regsTemps;
    int _regPing;
    int pingInterval;
    int _regStart;
    int _regStop;
    int _regStatus;
    int _regTargetTemp;
    int _regWriteTargetTemp;

    int _regPID_p;
    int _regPID_i;
    int _regPID_d;
    int regPID_write;

    int _regUptimeLo;
    int _regUptimeHi;

    int _regSteptimeLo;
    int _regSteptimeHi;

    float _valueTargetTemp;
    float _valuePIDp;
    float _valuePIDi;
    float _valuePIDd;
    int _valueUptimeLo;
    int _valueUptimeHi;
    int _valueSteptimeLo = 0;
    int _valueSteptimeHi = 0;




    QString ipAddr;
    int ipPort;
    int _ipModbusAddr;

    QModbusReply *lastReply;
    QModbusTcpClient *modbusClient;
    QModbusDataUnit pingDataUnit;
    void sendData(const int address, const int data);
    int readData(const int address);

    void startReconnect();
    int RECONNECT_INTERVAL = 3000;

    ModbusState state;

    void readReport();
    QTimer *pingTimer;

    void setupPing();
    void startPing();

    Status _status;
    int _statusCode;
    int _errorCode;


    qint32 from16Bit(qint16 high, qint16 low) const;
    quint16 hi_from32Bit(quint32 input) const;
    quint16 lo_from32Bit(quint32 input) const;



    QVector<qint16> _data;
    QVector<float> _valueTemperatures;

    void readDataRequest();
    void writeDataRequest();

    void setValues();


    QVector<float> fromInt100ToFloatQVector(const QVector<qint16> vector) const;

    bool _dataChanged;
    quint16 _dataChangedFrom;
    quint16 _dataChangedCount;
    QVector<qint16> _dataChangedNewData;

    void dataChanged(const quint16 from,
            const quint16 count,
            const QVector<qint16> changedData
            );

    void dataChangedOneValue(const quint16 addr, const qint16 value);


    QVector<qint16> dataModify(const QVector<qint16> sourceData,
            const quint16 from,
            const quint16 count,
            const QVector<qint16> changedData
            ) const;



    void statusParsing(quint16 statusCode);

    QVector<qint16> _dataToSend;




private slots:

    void onStateChanged(QModbusDevice::State state);
    void onError(QModbusDevice::Error error);

    void onControllerStatusReply();
    void onControllerPollFinished();
    void onConrollerPingFinished();
    void ping();


    void readDataResponse();
    void writeDataResponse();

};

#endif // CONTROLLER_H
