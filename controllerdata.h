#ifndef CONTROLLERDATA_H
#define CONTROLLERDATA_H

#include <QObject>
#include <QVector>

class ControllerData : public QObject
{
    Q_OBJECT
public:
    explicit ControllerData(QObject *parent = nullptr);

    qint16 temp1() const;
    void setTemp1(const qint16 &temp1);

    qint16 temp2() const;
    void setTemp2(const qint16 &temp2);

    qint16 temp3() const;
    void setTemp3(const qint16 &temp3);

    qint16 temp4() const;
    void setTemp4(const qint16 &temp4);

    qint16 temp5() const;
    void setTemp5(const qint16 &temp5);

    qint16 temp6() const;
    void setTemp6(const qint16 &temp6);

    qint16 temp7() const;
    void setTemp7(const qint16 &temp7);

    qint16 temp8() const;
    void setTemp8(const qint16 &temp8);

    quint16 pidP() const;
    void setPidP(const quint16 &pidP);

    quint16 pidI() const;
    void setPidI(const quint16 &pidI);

    quint16 pidD() const;
    void setPidD(const quint16 &pidD);

    quint16 writePid() const;
    void setWritePid(const quint16 &writePid);

    qint16 targetTemp() const;
    void setTargetTemp(const qint16 &targetTemp);

    quint16 writeTargetTemp() const;
    void setWriteTargetTemp(const quint16 &writeTargetTemp);

    quint16 start() const;
    void setStart(const quint16 &start);

    quint16 stop() const;
    void setStop(const quint16 &stop);

    quint16 status() const;
    void setStatus(const quint16 &status);

    quint16 uptimeLo() const;
    void setUptimeLo(const quint16 &uptimeLo);

    quint16 uptimeHi() const;
    void setUptimeHi(const quint16 &uptimeHi);

    quint16 steptimeLo() const;
    void setSteptimeLo(const quint16 &steptimeLo);

    quint16 steptimeHi() const;
    void setSteptimeHi(const quint16 &steptimeHi);

    quint16 ping() const;
    void setPing(const quint16 &ping);


    QVector<quint16> toModbusData() const;
    void fromModbusData(const QVector<quint16> data);

    qreal realTemp1() const;
    qreal realTemp2() const;
    qreal realTemp3() const;
    qreal realTemp4() const;
    qreal realTemp5() const;
    qreal realTemp6() const;
    qreal realTemp7() const;
    qreal realTemp8() const;

    void setRealTemp1(const qreal temp1);
    void setRealTemp2(const qreal temp2);
    void setRealTemp3(const qreal temp3);
    void setRealTemp4(const qreal temp4);
    void setRealTemp5(const qreal temp5);
    void setRealTemp6(const qreal temp6);
    void setRealTemp7(const qreal temp7);
    void setRealTemp8(const qreal temp8);

    quint32 fullUptimeSec() const;

    quint32 fullSteptimeSec() const;
    void setFullSteptime(const quint32 steptimeSec);

    qreal realPIDp() const;
    void setRealPIDp(const qreal value);

    qreal realPIDi() const;
    void setRealPIDi(const qreal value);

    qreal realPIDd() const;
    void setRealPIDd(const qreal value);

    qreal realTargetTemp() const;
    void setRealTargetTemp(const qreal value);





    bool changed() const;
    void setChanged(bool changed);

signals:

private:
    qint16 _temp1;
    qint16 _temp2;
    qint16 _temp3;
    qint16 _temp4;
    qint16 _temp5;
    qint16 _temp6;
    qint16 _temp7;
    qint16 _temp8;
    quint16 _pidP;
    quint16 _pidI;
    quint16 _pidD;
    quint16 _writePid;
    qint16 _targetTemp;
    quint16 _writeTargetTemp;
    quint16 _start;
    quint16 _stop;
    quint16 _status;
    quint16 _uptimeLo;
    quint16 _uptimeHi;
    quint16 _steptimeLo;
    quint16 _steptimeHi;
    quint16 _ping;


    bool _changed;


    int nint(double x) const;
    qreal int100ToReal(const int int100) const;
    quint16 realToUnsignedInt100(const qreal value) const;
    qint16 realToInt100(const qreal value) const;



public slots:
};

#endif // CONTROLLERDATA_H
