#include "controllerdata.h"

#include <QDebug>

ControllerData::ControllerData(QObject *parent) :
    QObject(parent),
    _temp1(0),
    _temp2(0),
    _temp3(0),
    _temp4(0),
    _temp5(0),
    _temp6(0),
    _temp7(0),
    _temp8(0),
    _pidP(0),
    _pidI(0),
    _pidD(0),
    _writePid(0),
    _targetTemp(0),
    _writeTargetTemp(0),
    _start(0),
    _stop(0),
    _status(0),
    _uptimeLo(0),
    _uptimeHi(0),
    _steptimeLo(0),
    _steptimeHi(0),
    _ping(0),
    _changed(false)

{

}



quint16 ControllerData::pidP() const
{
    return _pidP;
}

void ControllerData::setPidP(const quint16 &pidP)
{
    _pidP = pidP;
}

quint16 ControllerData::pidI() const
{
    return _pidI;
}

void ControllerData::setPidI(const quint16 &pidI)
{
    _pidI = pidI;
}

quint16 ControllerData::pidD() const
{
    return _pidD;
}

void ControllerData::setPidD(const quint16 &pidD)
{
    _pidD = pidD;
}

quint16 ControllerData::writePid() const
{
    return _writePid;
}

void ControllerData::setWritePid(const quint16 &writePid)
{
    _writePid = writePid;
}

qint16 ControllerData::targetTemp() const
{
    return _targetTemp;
}

void ControllerData::setTargetTemp(const qint16 &targetTemp)
{
    _targetTemp = targetTemp;
    this->setChanged(true);
}

quint16 ControllerData::writeTargetTemp() const
{
    return _writeTargetTemp;
}

void ControllerData::setWriteTargetTemp(const quint16 &writeTargetTemp)
{
    _writeTargetTemp = writeTargetTemp;
    this->setChanged(true);
}

quint16 ControllerData::start() const
{
    return _start;
}

void ControllerData::setStart(const quint16 &start)
{
    _start = start;
    this->setChanged(true);
}

quint16 ControllerData::stop() const
{
    return _stop;
}

void ControllerData::setStop(const quint16 &stop)
{
    _stop = stop;
    this->setChanged(true);
}

quint16 ControllerData::status() const
{
    return _status;
}

void ControllerData::setStatus(const quint16 &status)
{
    _status = status;
}

quint16 ControllerData::uptimeLo() const
{
    return _uptimeLo;
}

void ControllerData::setUptimeLo(const quint16 &uptimeLo)
{
    _uptimeLo = uptimeLo;
    this->setChanged(true);
}

quint16 ControllerData::uptimeHi() const
{
    return _uptimeHi;
}

void ControllerData::setUptimeHi(const quint16 &uptimeHi)
{
    _uptimeHi = uptimeHi;
    this->setChanged(true);
}

quint16 ControllerData::steptimeLo() const
{
    return _steptimeLo;
}

void ControllerData::setSteptimeLo(const quint16 &steptimeLo)
{
    _steptimeLo = steptimeLo;
    this->setChanged(true);
}

quint16 ControllerData::steptimeHi() const
{
    return _steptimeHi;
}

void ControllerData::setSteptimeHi(const quint16 &steptimeHi)
{
    _steptimeHi = steptimeHi;
    this->setChanged(true);
}

quint16 ControllerData::ping() const
{
    return _ping;
}

void ControllerData::setPing(const quint16 &ping)
{
    _ping = ping;
}

QVector<quint16> ControllerData::toModbusData() const
{

    QVector<quint16> data;

    data.append(static_cast<quint16>(_temp1));
    data.append(static_cast<quint16>(_temp2));
    data.append(static_cast<quint16>(_temp3));
    data.append(static_cast<quint16>(_temp4));
    data.append(static_cast<quint16>(_temp5));
    data.append(static_cast<quint16>(_temp6));
    data.append(static_cast<quint16>(_temp7));
    data.append(static_cast<quint16>(_temp8));

    data.append(_pidP);
    data.append(_pidI);
    data.append(_pidD);
    data.append(_writePid);

    data.append(static_cast<quint16>(_targetTemp));
    data.append(_writeTargetTemp);

    data.append(_start);
    data.append(_stop);
    data.append(_status);
    data.append(0); // register 17
    data.append(_uptimeLo);
    data.append(_uptimeHi);
    data.append(_steptimeLo);
    data.append(_steptimeHi);
    data.append(_ping);

    return data;

}

void ControllerData::fromModbusData(const QVector<quint16> data)
{
    _temp1 = static_cast<qint16>(data[0]);
    _temp2 = static_cast<qint16>(data[1]);
    _temp3 = static_cast<qint16>(data[2]);
    _temp4 = static_cast<qint16>(data[3]);
    _temp5 = static_cast<qint16>(data[4]);
    _temp6 = static_cast<qint16>(data[5]);
    _temp7 = static_cast<qint16>(data[6]);
    _temp8 = static_cast<qint16>(data[7]);

    _pidP = data[8];
    _pidI = data[9];
    _pidD = data[10];
    _writePid = data[11];

    _targetTemp = static_cast<qint16>(data[12]);
    _writeTargetTemp = data[13];

    _start = data[14];
    _stop = data[15];
    _status = data[16];
    // register 17
    _uptimeLo = data[18];
    _uptimeHi = data[19];
    _steptimeLo = data[20];
    _steptimeHi = data[21];
    _ping = data[22];

}

qreal ControllerData::realTemp1() const
{
    return int100ToReal(_temp1);
}

qreal ControllerData::realTemp2() const
{
    return int100ToReal(_temp2);
}

qreal ControllerData::realTemp3() const
{
    return int100ToReal(_temp3);
}

qreal ControllerData::realTemp4() const
{
    return int100ToReal(_temp4);
}

qreal ControllerData::realTemp5() const
{
    return int100ToReal(_temp5);
}

qreal ControllerData::realTemp6() const
{
    return int100ToReal(_temp6);
}

qreal ControllerData::realTemp7() const
{
    return int100ToReal(_temp7);
}

qreal ControllerData::realTemp8() const
{
    return int100ToReal(_temp8);
}

void ControllerData::setRealTemp1(const qreal temp1)
{
    _temp1 = realToInt100(temp1);
}

void ControllerData::setRealTemp2(const qreal temp2)
{
    _temp2 = realToInt100(temp2);
}

void ControllerData::setRealTemp3(const qreal temp3)
{
    _temp3 = realToInt100(temp3);
}

void ControllerData::setRealTemp4(const qreal temp4)
{
    _temp4 = realToInt100(temp4);
}

void ControllerData::setRealTemp5(const qreal temp5)
{
    _temp5 = realToInt100(temp5);
}

void ControllerData::setRealTemp6(const qreal temp6)
{
    _temp6 = realToInt100(temp6);
}

void ControllerData::setRealTemp7(const qreal temp7)
{
    _temp7 = realToInt100(temp7);
}

void ControllerData::setRealTemp8(const qreal temp8)
{
    _temp8 = realToInt100(temp8);
}

quint32 ControllerData::fullUptimeSec() const
{
    return static_cast<quint32>(_uptimeHi<<16) | _uptimeLo;
}

quint32 ControllerData::fullSteptimeSec() const
{
    return static_cast<quint32>(_steptimeHi<<16) |_steptimeLo;
}

void ControllerData::setFullSteptime(const quint32 steptimeSec)
{
    _steptimeLo = static_cast<quint16>(steptimeSec & 0xffff);
    _steptimeHi = static_cast<quint16>(steptimeSec>>16 & 0xffff);
    this->setChanged(true);
}

qreal ControllerData::realPIDp() const
{
    return int100ToReal(_pidP);
}

void ControllerData::setRealPIDp(const qreal value)
{
    _pidP = realToUnsignedInt100(value);
    this->setChanged(true);
}

qreal ControllerData::realPIDi() const
{
    return int100ToReal(_pidI);
}

void ControllerData::setRealPIDi(const qreal value)
{
    _pidI = realToUnsignedInt100(value);
    this->setChanged(true);
}

qreal ControllerData::realPIDd() const
{
    return int100ToReal(_pidD);
}

void ControllerData::setRealPIDd(const qreal value)
{
    _pidD = realToUnsignedInt100(value);
    this->setChanged(true);
}

qreal ControllerData::realTargetTemp() const
{
    return int100ToReal(_targetTemp);
}

void ControllerData::setRealTargetTemp(const qreal value)
{
    _targetTemp = realToInt100(value);
    this->setChanged(true);
}

bool ControllerData::changed() const
{
    return _changed;
}

void ControllerData::setChanged(bool changed)
{
    _changed = changed;
}

int ControllerData::nint(double x) const
{
    return x < 0 ? (int)ceil(x) : (int)floor(x);
}

qreal ControllerData::int100ToReal(const int int100) const
{
    return static_cast<qreal>(int100/100.0);
}

quint16 ControllerData::realToUnsignedInt100(const qreal value) const
{
    return static_cast<quint16>(nint(value*100));
}

qint16 ControllerData::realToInt100(const qreal value) const
{
    return static_cast<qint16>(nint(value*100));
}

qint16 ControllerData::temp1() const
{
    return _temp1;
}

void ControllerData::setTemp1(const qint16 &temp1)
{
    _temp1 = temp1;
}

qint16 ControllerData::temp2() const
{
    return _temp2;
}

void ControllerData::setTemp2(const qint16 &temp2)
{
    _temp2 = temp2;
}

qint16 ControllerData::temp3() const
{
    return _temp3;
}

void ControllerData::setTemp3(const qint16 &temp3)
{
    _temp3 = temp3;
}

qint16 ControllerData::temp4() const
{
    return _temp4;
}

void ControllerData::setTemp4(const qint16 &temp4)
{
    _temp4 = temp4;
}

qint16 ControllerData::temp5() const
{
    return _temp5;
}

void ControllerData::setTemp5(const qint16 &temp5)
{
    _temp5 = temp5;
}

qint16 ControllerData::temp6() const
{
    return _temp6;
}

void ControllerData::setTemp6(const qint16 &temp6)
{
    _temp6 = temp6;
}

qint16 ControllerData::temp7() const
{
    return _temp7;
}

void ControllerData::setTemp7(const qint16 &temp7)
{
    _temp7 = temp7;
}

qint16 ControllerData::temp8() const
{
    return _temp8;
}

void ControllerData::setTemp8(const qint16 &temp8)
{
    _temp8 = temp8;
}
