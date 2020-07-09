#include "cbackend.h"
#include <QDebug>
#include <QDateTime>
#include <qmath.h>

CBackend::CBackend(QObject *parent)
    : QObject(parent)
{

    m_tmrLog=new QTimer(this);
    m_tmrLog->setInterval(1);
    connect(m_tmrLog,&QTimer::timeout,this,&CBackend::OnTimeoutLog);

}

CBackend::~CBackend()
{
}

void CBackend::OnTimeoutLog()
{
    static double valX;
    double valY=(qSin(valX))*(180/3.141594);

    valX+=0.005;
    setSensor1(valY/22);

}

double CBackend::GetSensor1()
{
    return m_sensor1;

}

double CBackend::GetSensor2()
{
    return m_sensor2;
}


void CBackend::setSensor1(double val)
{
    if(val==m_sensor1)return;
    m_sensor1=val;


    emit sensor1Changed(m_sensor1);

}

void CBackend::setSensor2(double val)
{
    if(val==m_sensor2)return;
    m_sensor2=val;
    emit sensor1Changed(m_sensor2);
}

void CBackend::OnStart(bool run)
{
    m_run=run;
    if(m_run)m_tmrLog->start();
    else m_tmrLog->stop();
}
