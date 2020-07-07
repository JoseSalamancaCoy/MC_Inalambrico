#ifndef CBACKEND_H
#define CBACKEND_H

#include <QObject>
#include <QTimer>
class QSqlDatabase;

class CBackend : public QObject
{

    enum Tags{
       SENSOR1,
       SENSOR2
    };
    Q_OBJECT
public:
    explicit CBackend(QObject *parent = nullptr);
   ~CBackend();

    Q_PROPERTY(double sensor1 READ GetSensor1 WRITE setSensor1 NOTIFY sensor1Changed)
    Q_PROPERTY(double sensor2 READ GetSensor2 WRITE setSensor2 NOTIFY sensor2Changed)

signals:

    void tit101PointChanged(quint64 xval, double yval);

    void sensor1Changed(double val);
    void sensor2Changed(double val);


public slots:
    void OnTimeoutLog();
    double GetSensor1();
    double GetSensor2();


    void setSensor1(double val);
    void setSensor2(double val);



private:

    void InitDB(QString dbName);
    void logDataDB(QString tbl, uint16_t tag, double value);


    bool createTable( QString tbl);

    QTimer *m_tmrLog;

    QString m_currentDB;
    QString m_currentTbl;
    QSqlDatabase *m_db;

    double m_sensor1;
    double m_sensor2;




};

#endif // CBACKEND_H
