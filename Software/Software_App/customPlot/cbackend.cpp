#include "cbackend.h"
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>

#include <QDateTime>

#include <QSqlQuery>
#include <qmath.h>

CBackend::CBackend(QObject *parent)
    : QObject(parent)
{

    m_tmrLog=new QTimer(this);
    InitDB("dbData");

}

CBackend::~CBackend()
{
    m_db->close();
}

void CBackend::OnTimeoutLog()
{
    static double valX;
    double valY=(qSin(valX))*(180/3.141594);
    //logDataDB(m_currentTbl,SENSOR1,valY);
    //logDataDB(m_currentTbl,TIT_102,valY);
    valX+=0.005;
    setSensor1(valY/10);

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

void CBackend::InitDB(QString dbName)
{

    m_db= new QSqlDatabase(QSqlDatabase::addDatabase("QSQLITE"));
    m_db->setHostName("localhost");
    m_db->setDatabaseName("dbData.db");
    //m_db->setUserName("sei10");
    //m_db->setPassword("sei0");

   // m_currentDB=dbName;
    QSqlQuery query(*m_db);

    bool r =m_db->open();
    if(!r)
    {
        QSqlError err= m_db->lastError();
        qDebug() << "Open: "+ err.text();
        return;
    }

    /*
    r=query.exec("CREATE DATABASE IF NOT EXSITS " +  dbName +";");
    if(!r)
    {
        QSqlError err= m_db->lastError();
        qDebug() << "Crear database: "+ err.text();
        return;
    }
    r=query.exec("USE "+dbName);
    if(!r)
    {
        QSqlError err= m_db->lastError();
        qDebug() <<"USE Database "+ err.text();
        return;
    }

*/
    m_currentTbl="tbl_data";
    r=createTable(m_currentTbl);



    m_tmrLog->setInterval(1);
    connect(m_tmrLog,&QTimer::timeout,this,&CBackend::OnTimeoutLog);



}


void CBackend::logDataDB(QString tbl,uint16_t tag, double value)
{
    //QDateTime startTime;
    //startTime.setMSecsSinceEpoch(9e11);

    //QDateTime dtime(QDateTime::currentDateTime());

    //INSERT INTO tbl_data1(idtag, dataFloat,timeStamp) VALUES (10, 10.5, 1012
    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO "+ tbl +" (idtag, dataFloat) "
                  "VALUES (:idtag, :dataFloat)");
    query.bindValue(":idtag", tag);
    query.bindValue(":dataFloat", value);
    //query.bindValue(":timeStamp",startTime.msecsTo(dtime));



    if(!query.exec())
    {
        QSqlError err =m_db->lastError();
        qDebug()<< err.text();
        return;

    }
    //else ;//qDebug() << "timeStam: " << startTime.msecsTo(dtime);
   // qDebug()<< startTime;
  //  qDebug()<< dtime;


}

bool CBackend::createTable(QString tbl)
{
    /*
    CREATE TABLE `tbl_data` (
        `ID`	INTEGER PRIMARY KEY AUTOINCREMENT,
        `idTag`	INTEGER NOT NULL,
        `value`	REAL
    );
    */

    QSqlQuery query(*m_db);
    QString strQuery;
    strQuery=("CREATE TABLE IF NOT EXISTS "+tbl+
              " ("
              "ID	INTEGER PRIMARY KEY AUTOINCREMENT,"
              " idtag INT UNSIGNED  NOT NULL,"
              " dataFloat REAL NULL"
              " );");
    query.prepare(strQuery);

    if(!query.exec())
    {
        QSqlError err=m_db->lastError();
        qDebug() << err.text();
        return false;
    }

    return  true;
}
