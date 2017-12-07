#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QtSql>
#include <QDateTime>
#include <QSqlTableModel>


class Database : public QObject
{
    Q_OBJECT
public:
    explicit Database(const QString &databasePath, QObject *parent = nullptr);
    ~Database();
    bool openError() const;
    QString errorString() const;

    void saveValues(const QString &datetime,
            const QString &uptime,
            const QString &steptime,
            const QString targetTemp,
            const QString currentTemp
            );
    
    QSqlTableModel *getModel() const;

    

signals:
    void errorOccured(const QString errorString);

public slots:


private:
    QSqlDatabase _db;
    bool _openError;
    QString _errorString;
    QSqlTableModel *_valuesModel;
    QSqlTableModel *_viewModel;

};

#endif // DATABASE_H
