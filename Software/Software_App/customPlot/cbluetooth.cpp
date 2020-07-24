#include "cbluetooth.h"
#include <QtCore/qmetaobject.h>
#include <QDataStream>


typedef union
{
  struct
  {
      uint16_t valY;
      uint32_t valX;      
  };
  char values[8];
}user_data;


CBluetooth::CBluetooth(QObject *parent)
    : QObject(parent)
{

}

CBluetooth::~CBluetooth()
{
    stopBluetooth();
}

void CBluetooth::startBluetooth(const QBluetoothServiceInfo &remoteService)
{
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    qDebug() << "Creando socket";
    socket->connectToService(remoteService);
    qDebug() << "Servisio Conectado";

    connect(socket, &QBluetoothSocket::readyRead, this, &CBluetooth::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&CBluetooth::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &CBluetooth::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &CBluetooth::onSocketErrorOccurred);
}

void CBluetooth::stopBluetooth()
{
    delete socket;
    socket = nullptr;
}

void CBluetooth::sendMessage(const QString &message)
{
    QByteArray text = message.toUtf8() + '\n';
    socket->write(text);

}

void CBluetooth::readSocket()
{

    if (!socket)
        return;

    user_data data;


    socket->read(data.values,sizeof (user_data));
   // qDebug() << data.valX << data.valY ;

    if(data.valY & 0x8000)
    {
        emit bateryChanged((data.valY&0x0fff)*0.001221);
    }
    else
    {
        emit dataChanged(data.valX,data.valY);
    }




}

void CBluetooth::connected()
{
    emit connected(socket->peerName());
}

void CBluetooth::onSocketErrorOccurred(QBluetoothSocket::SocketError error)
{
    if (error == QBluetoothSocket::NoSocketError)
        return;

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString errorString = socket->peerName() + QLatin1Char(' ')
            + metaEnum.valueToKey(error) + QLatin1String(" occurred");

    emit socketErrorOccurred(errorString);

}
