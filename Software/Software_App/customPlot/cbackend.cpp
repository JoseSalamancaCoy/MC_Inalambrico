// Simulador para generar datos y manejador de base de datos

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
    //logDataDB(m_currentTbl,LIT_101,valY);
    //logDataDB(m_currentTbl,TIT_102,valY);
    valX+=0.005;
    setSensor1(valY);

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

void CBackend::InitDB(QString dbName)
{

    m_db= new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL","connectio1"));
    m_db->setHostName("localhost");
    m_db->setDatabaseName("dbData");
    m_db->setUserName("sei10");
    m_db->setPassword("sei0");

    m_currentDB=dbName;
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
    m_currentTbl="tbl_data2";
    r=createTable(m_currentTbl);



    m_tmrLog->setInterval(1);
    connect(m_tmrLog,&QTimer::timeout,this,&CBackend::OnTimeoutLog);
    m_tmrLog->start();


}


void CBackend::logDataDB(QString tbl,uint16_t tag, double value)
{
    QDateTime startTime;
    startTime.setMSecsSinceEpoch(9e11);

    QDateTime dtime(QDateTime::currentDateTime());

    //INSERT INTO tbl_data1(idtag, dataFloat,timeStamp) VALUES (10, 10.5, 1012
    QSqlQuery query(*m_db);
    query.prepare("INSERT INTO "+ tbl +" (idtag, dataFloat,timeStamp) "
                  "VALUES (:idtag, :dataFloat, :timeStamp)");
    query.bindValue(":idtag", tag);
    query.bindValue(":dataFloat", value);
    query.bindValue(":timeStamp",startTime.msecsTo(dtime));



    if(!query.exec())
    {
        QSqlError err =m_db->lastError();
        qDebug()<< err.text();
        return;

    }
    else ;//qDebug() << "timeStam: " << startTime.msecsTo(dtime);
   // qDebug()<< startTime;
  //  qDebug()<< dtime;


}

bool CBackend::createTable(QString tbl)
{
    QSqlQuery query(*m_db);
    query.prepare("CREATE TABLE IF NOT EXISTS "+ m_currentDB+"."+tbl+ " ("
                    " idtag INT UNSIGNED  NOT NULL,"
                    " dataInt INT NULL,"
                    " dataFloat REAL NULL,"
                    " dataString varchar(32) NULL,"
                    " dataBool BOOL NULL,"
                    " timeStamp BIGINT UNSIGNED NOT NULL  PRIMARY KEY"
                    " )"

                    " ENGINE=InnoDB"
                    " DEFAULT CHARSET=utf8mb4"
                    " COLLATE=utf8mb4_general_ci;"
                    " CREATE UNIQUE INDEX tbl_data1_timeStamp_IDX USING BTREE ON dbtest.tbl_data1 (`timeStamp`);"
                    " CREATE INDEX tbl_data1_idtag_IDX USING BTREE ON dbtest.tbl_data1 (idtag););");

    if(!query.exec())
    {
        QSqlError err=m_db->lastError();
        qDebug() << err.text();
        return false;
    }

    return  true;
}
