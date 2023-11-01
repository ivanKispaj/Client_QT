
#pragma once

#include <QWidget>
#include "tcpclient.h"
#include "User.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class RegistrationWidget;
}
QT_END_NAMESPACE

class RegistrationWidget : public QWidget
{
    Q_OBJECT

public:
    explicit RegistrationWidget(QWidget *parent = nullptr);
    ~RegistrationWidget();
    void setNetworkService(TCPClient *network);
signals:
    void loginRequesterd();
    void accepted(const User &user);
    void rejected();
private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_loginButton_clicked();

    void networkRequest(const QByteArray &data);

private:
    Ui::RegistrationWidget *ui;
    TCPClient *_network{nullptr};
    bool verifyFormFields();

};

