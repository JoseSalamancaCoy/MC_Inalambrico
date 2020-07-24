#ifndef CBLUETOOTH_H
#define CBLUETOOTH_H

#include <QObject>

#include <QtBluetooth/qbluetoothserviceinfo.h>
#include <QtBluetooth/qbluetoothsocket.h>
#include <stdint.h>

QT_FORWARD_DECLARE_CLASS(QBluetoothSocket)

QT_USE_NAMESPACE

class CBluetooth : public QObject
{
    Q_OBJECT
public:
    explicit CBluetooth(QObject *parent = nullptr);
    ~CBluetooth();

    void startBluetooth(const QBluetoothServiceInfo &remoteService);
    void stopBluetooth();

public slots:
    void sendMessage(const QString &message);

signals:
    void messageReceived(const double &Y,const double &X);
    void dataChanged(const uint32_t &valX, const uint16_t &valY);
    void connected(const QString &name);
    void disconnected();
    void socketErrorOccurred(const QString &errorString);
    void bateryChanged(double value);

private slots:
    void readSocket();
    void connected();
    void onSocketErrorOccurred(QBluetoothSocket::SocketError error);

private:
    QBluetoothSocket *socket = nullptr;

};

#endif // CBLUETOOTH_H
