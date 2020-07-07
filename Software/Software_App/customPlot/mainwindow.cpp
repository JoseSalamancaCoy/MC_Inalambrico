/* Contiene la ventana principal y configuracion del plotter
    Para este proyecto se utiliza customplot para mejorar el rendimiento
    Se escribe directamente en archivos .CSV
*/

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h"
#include <QVector>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{


    ui->setupUi(this);

    m_indX=0;


    m_backend=new CBackend(this);
    connect(m_backend,&CBackend::sensor1Changed,this,&MainWindow::OnNewData);



    ui->ploter->addGraph();
    ui->ploter->graph()->setPen(QPen(Qt::blue));
    ui->ploter->graph()->setBrush(QBrush(QColor(0, 0, 255, 20)));


/*
    for (int i=0; i<500; ++i)
      {
        x[i] = (i/499.0-0.5)*10;
        y[i] = qExp(-x[i]*x[i]*0.25)*qSin(x[i]*5)*5;

      }
*/
      ui->horizontalScrollBar->setRange(0, 500);
      ui->verticalScrollBar->setRange(-100, 100);

      // create connection between axes and scroll bars:
      connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
      connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
      connect(ui->ploter->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
      connect(ui->ploter->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));

      // initialize axis range (and scroll bar positions via signals we just connected):
      ui->ploter->xAxis->setRange(0, 100, Qt::AlignLeft);
      ui->ploter->yAxis->setRange(-100, 100, Qt::AlignLeft);

      //ui->ploter->graph(0)->setData(x, y);
      ui->ploter->axisRect()->setupFullAxesBox(true);
      ui->ploter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);

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
    ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
    ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider

}

void MainWindow::yAxisChanged(QCPRange range)
{
    ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
    ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void MainWindow::OnNewData(float val)
{

    if(m_XValues.length()>=10000)
    {
        m_YValues.push_back(val);
        m_YValues.pop_front();

        m_XValues.push_back(m_indX++);
        m_XValues.pop_front();

        ui->ploter->xAxis->setRange(m_XValues.first(), m_XValues.last());

        //qDebug() << "zise: "<< ui->ploter->xAxis->range().size();

        //qDebug() << "length: " << m_XValues.length();

    }
    else
    {
        m_XValues.append(m_indX++);
        m_YValues.push_back(val);
        ui->ploter->xAxis->setRange(m_XValues.first(), m_XValues.last(), Qt::AlignLeft);
        //qDebug() << "yVal: " <<val  << "xVal: " << m_indX;
    }




    ui->ploter->graph(0)->setData(m_XValues, m_YValues);
    //ui->ploter->axisRect()->setupFullAxesBox(true);
    ui->ploter->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
    ui->ploter->replot();

}



