

#include "tcpclient.h"
#include <QJsonDocument>
#include <QJsonArray>

TCPClient::TCPClient(QObject *parent)
    : QObject(parent), _socket(nullptr)
{
    _socket = new QTcpSocket(this);
    connect(_socket, &QTcpSocket::disconnected, this, &TCPClient::onDisconnected);
    connect(_socket, &QTcpSocket::readyRead, this, &TCPClient::handleResponse);
    _socket->connectToHost(_host, _port);
}

TCPClient::~TCPClient()
{
    if (_socket->isOpen())
    {
        _socket->close();
    }
    delete _socket;
}

void TCPClient::sendResponse(uint16_t command,const QJsonObject &data)
{
    QJsonObject sendObj(data);
    if (_socket->isOpen())
    {
        sendObj["command"] = QString::number(command);
        QJsonDocument doc{sendObj};
        QThread *thread = new QThread(this);
        connect(thread, &QThread::started, this, [this, doc, thread]()
                {
                    _socket->write(doc.toJson());
                    _socket->waitForBytesWritten();
                    _socket->waitForReadyRead();
                    emit responseReceived(_data);
                    thread->quit();
                });
        connect(thread, &QThread::finished, thread, &QThread::deleteLater);
        thread->start();
    }
}

bool TCPClient::isConnected()
{
    return _socket->isOpen();
}

void TCPClient::handleResponse()
{

    QByteArray data = _socket->readAll();
    emit responseReceived(data);
}

void TCPClient::onDisconnected()
{
    while (isConnected()){}
}

