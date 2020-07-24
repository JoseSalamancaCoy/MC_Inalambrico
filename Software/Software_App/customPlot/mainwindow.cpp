#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "cbluetooth.h"
#include "qcustomplot.h"
#include <QVector>
#include <QFile>
#include <QTimer>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>

#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);
    statusLabel=new QLabel();
    statusLabel->setText("Estado Bluetooth");
    statusLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    lblIndX=new QLabel();
    lblIndX->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    lblVoltaje=new QLabel();
    lblVoltaje->setFrameStyle(QFrame::Panel | QFrame::Sunken);

    ui->statusbar->addWidget(statusLabel,2);
    ui->statusbar->addWidget(lblIndX,3);
    ui->statusbar->addWidget(lblVoltaje,4);

    QTimer *tmrRefresh=new QTimer(this);
    tmrRefresh->setInterval(100);
    connect(tmrRefresh,&QTimer::timeout, this,&MainWindow::OnTimeout);
    tmrRefresh->start();

    m_indX=0;
    m_RangePlot=60000;
    m_salvar=false;

    QString nameFile;
    nameFile="file-"+ QDateTime::currentDateTime().toString("MM-dd-yyyy-HH_mm")+".csv";
    qDebug() << nameFile;

    m_file=new QFile(nameFile);

    ui->ploter->addGraph();
    ui->ploter->graph()->setPen(QPen(Qt::blue));

     connect(ui->ploter->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
     connect(ui->ploter->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

      // initialize axis range (and scroll bar positions via signals we just connected):
      QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
      ui->ploter->xAxis->setTicker(timeTicker);
      timeTicker->setTimeFormat("%h:%m:%s");

      ui->ploter->xAxis->setRange(0, 60, Qt::AlignLeft);
      ui->ploter->yAxis->setRange(-1, 20, Qt::AlignBaseline);

      //ui->ploter->graph(0)->setData(x, y);
      ui->ploter->axisRect()->setupFullAxesBox(true);
      ui->ploter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
      m_run_tendencia=false;

      ui->actionGuardar->setEnabled(false);
      ui->menuTendencia->setEnabled(false);
      ui->actionReiniciar->setEnabled(false);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::horzScrollBarChanged(int value)
{
    if (qAbs(ui->ploter->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
     {
       ui->ploter->xAxis->setRange(value/100.0, ui->ploter->xAxis->range().size(), Qt::AlignLeft);
       ui->ploter->replot();
     }

}

void MainWindow::vertScrollBarChanged(int value)
{
    if (qAbs(ui->ploter->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
     {
       ui->ploter->yAxis->setRange(-value/100.0, ui->ploter->yAxis->range().size(), Qt::AlignCenter);
       ui->ploter->replot();
     }

}

void MainWindow::xAxisChanged(QCPRange range)
{

}

void MainWindow::yAxisChanged(QCPRange range)
{
}
/*
void MainWindow::OnNewData(float valY, float valX)
{
    if(m_salvar)return;

    //static QTime time(QTime::currentTime());
    // calculate two new data points:
    //double mSeconds = time.elapsed()/1000.0; // time elapsed since start , in seconds

    //qDebug()<< time.toString("hh:mm:ss:ms");
    //qDebug() << m_XValues.length();

    m_XValues.push_back(valX);
    m_YValues.push_back(valY);

    if(m_XValues.length()>=m_RangePlot)
    {


        ui->ploter->xAxis->setRange(m_XValues.first()+m_indX,ui->ploter->xAxis->range().size(),Qt::AlignLeft);

        //qDebug() << "zise: "<< m_XValues.first();
        //qDebug() << "length: " << m_XValues.length();
        //m_indX+=0.001;

    }
    else
    {

        ui->ploter->xAxis->setRange(m_XValues.first(),ui->ploter->xAxis->range().size(), Qt::AlignLeft);

    }

    ui->ploter->graph(0)->addData(valX,valY);
   // ui->ploter->graph(0)->rescaleKeyAxis(true);




    //ui->ploter->graph(0)->setData(m_XValues, m_YValues);
    //ui->ploter->axisRect()->setupFullAxesBox(true);
    //ui->ploter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);




}
*/
void MainWindow::OnNewData(uint32_t valX, uint16_t valY)
{
    if(m_salvar)return;

    double valx=valX/1000.0;
    double valy=valY/1000.0;

    m_XValues.push_back(valx);
    m_YValues.push_back(valy);

   // qDebug() << valx << valy << lblVoltaje->text();

    if(m_XValues.length()>=m_RangePlot)
    {


        ui->ploter->xAxis->setRange(m_XValues.first()+m_indX,ui->ploter->xAxis->range().size(),Qt::AlignLeft);

        //qDebug() << "zise: "<< m_XValues.first();
        //qDebug() << "length: " << m_XValues.length();
         m_indX+=0.001;

    }
    else
    {

        ui->ploter->xAxis->setRange(m_XValues.first(),ui->ploter->xAxis->range().size(), Qt::AlignLeft);

    }

    //ui->ploter->graph(0)->addData(valX,valy);


}

void MainWindow::OnNewDataBatery(double value)
{
    lblVoltaje->setText("Bateria: " + QString::number(value)+" V");
}


void MainWindow::on_actionSalir_triggered()
{
    this->close();
}

void MainWindow::on_actionGuardar_triggered()
{
    m_salvar=true;

    if (!m_file->open(QIODevice::Append | QIODevice::Text))
        return;

    QTextStream textStream(m_file);
    qDebug() << "No Datos" << (m_XValues.length()* 16) << " Bytes";


    for (int var = 0; var < m_XValues.length(); ++var)
    {
        textStream << QString::number(m_XValues[var]) << "," << QString::number(m_YValues[var]) << "\n\r" ;
    }

     m_salvar=false;

    m_file->close();

}

void MainWindow::on_actionIniciar_triggered()
{
    m_run_tendencia=true;
    if (!m_file->open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream textStream(m_file);

    QDateTime dt(QDateTime::currentDateTime());


     textStream << "# Archivo para alamacenamiento de datos \n\r";
     textStream << dt.toString("dd-MM-yyyy hh:mm:ss");
     textStream << "# Value X, Value Y \n\r";

     m_file->close();
}

void MainWindow::on_actionParar_triggered()
{
    m_run_tendencia=false;
    //m_VectorFileX.clear();
    //m_VectorFileY.clear();
    //m_XValues.clear();
   // m_YValues.clear();
   // m_indX=0;

}

void MainWindow::OnTimeout()
{

    if(m_run_tendencia)
    {
        ui->ploter->graph(0)->setData(m_XValues,m_YValues);
        ui->ploter->replot();
    }
    lblIndX->setText("No Datos: "+QString::number(m_XValues.length()));
}

void MainWindow::on_actionConectar_triggered()
{
    //Bluetooth
    // scan for services
    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                           QBluetoothAddress() :
                                           localAdapters.at(currentAdapterIndex).address();

     BluSelector bluetoothSelector(adapter);
     bluetoothSelector.startDiscovery(QBluetoothUuid(QBluetoothUuid::SerialPort));

     if (bluetoothSelector.exec() == QDialog::Accepted) {
         QBluetoothServiceInfo service = bluetoothSelector.service();

         qDebug() << "Conectando al servisio 2" << service.serviceName()
                  << "on" << service.device().name();

         // Create client
         qDebug() << "Creando una conexion";
         m_blutooth = new CBluetooth(this);
 qDebug() << "Conectando...";

         connect(m_blutooth, &CBluetooth::dataChanged,
                 this, &MainWindow::OnNewData);
         connect(m_blutooth, &CBluetooth::disconnected,
                 this, QOverload<>::of(&MainWindow::bluetoothDisconnected));
         connect(m_blutooth, QOverload<const QString &>::of(&CBluetooth::connected),
                 this, &MainWindow::connected);
         connect(m_blutooth, &CBluetooth::socketErrorOccurred,
                 this, &MainWindow::reactOnSocketError);
         connect(this, &MainWindow::sendMessage, m_blutooth, &CBluetooth::sendMessage);

         connect(m_blutooth,&CBluetooth::bateryChanged,this, &MainWindow::OnNewDataBatery);

 qDebug() << "Start client";
         m_blutooth->startBluetooth(service);

         devices.append(m_blutooth);
     }

     //ui->connectButton->setEnabled(true);

    ui->horizontalSlider->setMaximum(60);
    ui->horizontalSlider->setMinimum(2);
    ui->horizontalSlider->setValue(30);
}

void MainWindow::sendClicked()
{

}

void MainWindow::showMessage(const QString &sender, const QString &message)
{

}

void MainWindow::bluetoothConnected(const QString &name)
{



}

void MainWindow::bluetoothDisconnected(const QString &name)
{

}

void MainWindow::bluetoothDisconnected()
{
    CBluetooth *blue = qobject_cast<CBluetooth *>(sender());
    if (blue)
    {
        devices.removeOne(blue);
        blue->deleteLater();
    }


}

void MainWindow::connected(const QString &name)
{
    qDebug() << "Conectado A:" << name;
    statusLabel->setStyleSheet("QLabel { background-color : white; color : green; }");
    statusLabel->setText("Conectado: " + name);

    ui->actionGuardar->setEnabled(true);
    ui->menuTendencia->setEnabled(true);
    on_actionIniciar_triggered();
}

void MainWindow::reactOnSocketError(const QString &error)
{

}

void MainWindow::newAdapterSelected()
{

}


void MainWindow::on_actionReiniciar_triggered()
{
    m_XValues.clear();
    m_YValues.clear();
    m_indX=0;

    ui->actionGuardar->setEnabled(false);
    ui->menuTendencia->setEnabled(false);
    ui->actionReiniciar->setEnabled(false);
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    //m_PositionX=position;
    if (qAbs(ui->ploter->xAxis->range().center()-position) > 0.01) // if user is dragging plot, we don't want to replot twice
    {
        ui->ploter->xAxis->setRange(m_XValues.first(), position, Qt::AlignLeft);
        ui->ploter->replot();
        qDebug()<< "Posi: " << position << " range :" <<  ui->ploter->xAxis->range().size();
    }

}

void MainWindow::on_actionDesconectar_triggered()
{
    m_blutooth->stopBluetooth();
    on_actionReiniciar_triggered();

    statusLabel->setStyleSheet("QLabel { background-color : white; color : red; }");
    statusLabel->setText("Desconectado ");

}
