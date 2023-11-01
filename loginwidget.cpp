#include "loginwidget.h"
#include "ui_loginwidget.h"
#include <QMessageBox>
#include "encodepassword.h"
#include <QJsonDocument>

LoginWidget::LoginWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::LoginWidget)
{
    ui->setupUi(this);
}

LoginWidget::~LoginWidget() { delete ui; }

void LoginWidget::setNetworkService(TCPClient *network)
{
    _network = network;
    connect(_network, &TCPClient::responseReceived, this, &LoginWidget::networkRequest);
}

void LoginWidget::on_registrationButton_clicked()
{
    emit registrationRequesterd();
}

void LoginWidget::on_buttonBox_accepted()
{
    if (verifyFormFields())
    {
        User user;
        user.setUserLogin(ui->loginEdit->text());
        std::string pass = ui->passwordEdit->text().toStdString();
        EncodePassword::sha1(pass);
        user.saveUserPassword(QString::fromStdString(pass));
        QJsonObject obj;
        obj["user"] = user.encodeToJson();
        _network->sendResponse(TCPClient::AUTHENTIFICATE, obj);
    } else
    {
        QMessageBox::critical(this, tr("Error"), tr("All fields of the form must be filled in!"));
    }
}

void LoginWidget::on_buttonBox_rejected() { emit rejected(); }

void LoginWidget::networkRequest(const QByteArray &data)
{
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    if (!obj.isEmpty() && obj.contains("auth_state"))
    {
        if (obj["auth_state"].toBool())
        {
            User user;
            user.decodeFromJson(obj["user"].toObject());
            emit accepted(user);
        } else
        {
            QMessageBox::critical(this, tr("Error"), tr("Authentication error!"));
        }
    }
}

bool LoginWidget::verifyFormFields()
{
    if (!ui->loginEdit->text().isEmpty() && !ui->passwordEdit->text().isEmpty())
    {
        return true;
    }
    return false;
}
