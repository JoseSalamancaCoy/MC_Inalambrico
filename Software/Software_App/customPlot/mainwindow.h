#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cbackend.h"
#include "qcustomplot.h"
#include <QPointer>

#include <QMainWindow>

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


private slots:
  void horzScrollBarChanged(int value);
  void vertScrollBarChanged(int value);
  void xAxisChanged(QCPRange range);
  void yAxisChanged(QCPRange range);

  void OnNewData(float val);


private:
    Ui::MainWindow *ui;
    QVector<double> m_XValues;
    QVector<double> m_YValues;
    CBackend *m_backend;
    uint32_t m_indX;



};
#endif // MAINWINDOW_H
