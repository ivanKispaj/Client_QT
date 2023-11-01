
#pragma once
#include <QWidget>
#include "tcpclient.h"
#include "User.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class LoginWidget;
}
QT_END_NAMESPACE

class LoginWidget : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWidget(QWidget *parent = nullptr);
    ~LoginWidget();
    void setNetworkService(TCPClient *network);
signals:
    void registrationRequesterd();
    void accepted(const User &user);
    void rejected();
private slots:
    void on_registrationButton_clicked();
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();
    void networkRequest(const QByteArray &data);

private:
    Ui::LoginWidget *ui;
    bool verifyFormFields();
    TCPClient *_network{nullptr};
};

