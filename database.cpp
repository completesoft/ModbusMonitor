#include "database.h"
#include <QSqlRecord>

Database::Database(const QString &databasePath, QObject *parent) :
    QObject(parent),
    _openError(false)
{
    _db = QSqlDatabase::addDatabase("QSQLITE");
    _db.setDatabaseName(databasePath);
    if (!_db.open()) {
        _openError = true;
        _errorString = _db.lastError().text();
        emit errorOccured("Ошибка подключения к БД " + _errorString);
        return;
    }

    _valuesModel = new QSqlTableModel(this);
    _valuesModel->setTable("data");


    _viewModel = new QSqlTableModel(this);
    _viewModel->setTable("data");
    _viewModel->setHeaderData(0, Qt::Horizontal, "Дата/время");
    _viewModel->setHeaderData(1, Qt::Horizontal, "Время работы");
    _viewModel->setHeaderData(2, Qt::Horizontal, "Время шага");
    _viewModel->setHeaderData(3, Qt::Horizontal, "Заданная температура");
    _viewModel->setHeaderData(4, Qt::Horizontal, "Текущая температура");


}

Database::~Database()
{
    _db.close();
}



bool Database::openError() const
{
    return _openError;
}

QString Database::errorString() const
{
    return _errorString;
}

void Database::saveValues(const QString &datetime,
        const QString &uptime,
        const QString &steptime,
        const QString targetTemp,
        const QString currentTemp
        )
{
    if (!_db.isOpen()) {
        emit errorOccured("Попытка записи при закрытой БД");
        return;
    }

    QSqlRecord record = _valuesModel->record();



    record.setValue("date_time", datetime);
    record.setValue("uptime", uptime);
    record.setValue("steptime", steptime);
    record.setValue("targettemp", targetTemp);
    record.setValue("currenttemp", currentTemp);

    _valuesModel->insertRecord(-1, record);



}

QSqlTableModel *Database::getModel() const
{
    return _viewModel;

}
