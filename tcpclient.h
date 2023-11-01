

#pragma once
#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QJsonObject>

class TCPClient : public QObject
{
    Q_OBJECT
public:
    explicit TCPClient(QObject *parent = nullptr);
    ~TCPClient();

    enum CommandToServer : uint16_t {
        ADD_USER = 2,
        ADD_MESSAGE = 4,
        IS_UNIQUE_LOGIN = 8,
        GET_ALL_USERS = 16,
        GET_USER_BY_LOGIN = 32,
        GET_USER_BY_ID = 64,
        USERS_COUNT = 128,
        GET_ALL_PRIVATE_MESSAGE = 256,
        UPDATE_USER_DATA = 512,
        DELETE_MESSAGE_BY_ID = 1024,
        GET_ALL_PUBLIC_MESSAGES = 2048,
        AUTHENTIFICATE = 4096,
    };

    void sendResponse(uint16_t command, const QJsonObject &data = QJsonObject());
    bool isConnected();

signals:
    void responseReceived(const QByteArray &response);
private slots:
    void handleResponse();
    void onDisconnected();

private:
    QTcpSocket *_socket;
    const QString _host = "localhost";
    const int _port = 45000;
    QByteArray _data;
};
