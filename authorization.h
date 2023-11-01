
#pragma once

#include <QDialog>
#include "tcpclient.h"
#include "User.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Authorization;
}
QT_END_NAMESPACE

class Authorization : public QDialog
{
    Q_OBJECT

signals:
    void onLogined(const User &user);

public:
    explicit Authorization(TCPClient *network, QWidget *parent = nullptr);
    ~Authorization();
    void setLoginForm();
    void setRegistrationForm();

private slots:
    void onLoginedUser(const User &user);
    void rejected();

private:
    Ui::Authorization *ui;
    TCPClient *_network{nullptr};
};

