#ifndef MAINWINDOW_H
#define MAINWINDOW_H
//==================================
#include "bluselector.h"
#include <QBluetoothHostInfo>

//=================================
#include "cbackend.h"
#include "cbluetooth.h"
#include "qcustomplot.h"
#include <QPointer>

//=================================

#include <QMainWindow>
class QFile;
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
class CBackend;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void sendMessage(const QString &message);


private slots:
  void horzScrollBarChanged(int value);
  void vertScrollBarChanged(int value);
  void xAxisChanged(QCPRange range);
  void yAxisChanged(QCPRange range);
  void OnNewData(float valY, float valX);
  void on_actionSalir_triggered();
  void on_actionGuardar_triggered();

  void on_actionIniciar_triggered();

  void on_actionParar_triggered();



  //Bluetoot
  //===============================

  void on_actionConectar_triggered();
  void sendClicked();
  void showMessage(const QString &sender, const QString &message);
  void bluetoothConnected(const QString &name);
  void bluetoothDisconnected(const QString &name);
  void bluetoothDisconnected();
  void connected(const QString &name);
  void reactOnSocketError(const QString &error);
  void newAdapterSelected();

  void on_actionReiniciar_triggered();

  void on_horizontalSlider_sliderMoved(int position);

private:
    Ui::MainWindow *ui;

    //Bluetooth
    QList<QBluetoothHostInfo> localAdapters;
    QList<CBluetooth *> devices;
    int currentAdapterIndex = 0;

    CBluetooth *m_blutooth;


    //===========================================
    //  Grafica
    //===========================================
    QVector<double> m_XValues;
    QVector<double> m_YValues;
    QVector<double> m_VectorFileX;
    QVector<double> m_VectorFileY;
    CBackend *m_backend;
    int m_indX;
    QFile *m_file;
    bool m_run_tendencia;
    int m_RangePlot;
    int m_PositionX;






};
#endif // MAINWINDOW_H
