#ifndef CBACKEND_H
#define CBACKEND_H

#include <QObject>
#include <QTimer>


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


    double GetSensor1();
    double GetSensor2();

signals:

    void tit101PointChanged(quint64 xval, double yval);

    void sensor1Changed(double val);
    void sensor2Changed(double val);


public slots:
    void OnTimeoutLog();

    void setSensor1(double val);
    void setSensor2(double val);
    void OnStart(bool run);



private:


    QTimer *m_tmrLog;

    QString m_currentDB;
    QString m_currentTbl;


    double m_sensor1;
    double m_sensor2;

    bool m_run;




};

#endif // CBACKEND_H
