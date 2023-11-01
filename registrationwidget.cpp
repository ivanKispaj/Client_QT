
#include "registrationwidget.h"
#include "ui_registrationwidget.h"
#include <QMessageBox>
#include "User.h"
#include "encodepassword.h"

RegistrationWidget::RegistrationWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::RegistrationWidget)
{
    ui->setupUi(this);
}

RegistrationWidget::~RegistrationWidget() { delete ui; }

void RegistrationWidget::setNetworkService(TCPClient *network)
{
    _network = network;
}

bool RegistrationWidget::verifyFormFields()
{
    if (!ui->loginTextEdit->text().isEmpty() &&
        !ui->passwordTextEdit->text().isEmpty() &&
        !ui->confirmePassTextEdit->text().isEmpty() &&
        !ui->nameTextEdit->text().isEmpty())
    {
        if (ui->passwordTextEdit->text() == ui->confirmePassTextEdit->text())
        {
            return true;
        } else
        {
            QMessageBox::critical(this, tr("Error"), tr("Passwords don't match!"));
        }
    } else
    {
        QMessageBox::critical(this, tr("Error"), tr("All fields a nidded fills!"));
    }
    return false;
}

void RegistrationWidget::on_buttonBox_accepted()
{
    if (verifyFormFields())
    {
        User user;
        std::string pass = ui->passwordTextEdit->text().toStdString();
        EncodePassword::sha1(pass);
        user.saveUserPassword(QString::fromStdString(pass));
        user.setUserLogin(ui->loginTextEdit->text());
        user.setUserName(ui->nameTextEdit->text());

        QJsonObject obj;
        obj["user"] = user.encodeToJson();
        _network->sendResponse(TCPClient::ADD_USER, obj);
    }
}

void RegistrationWidget::on_buttonBox_rejected() { emit rejected(); }

void RegistrationWidget::on_loginButton_clicked() { emit loginRequesterd(); }

void RegistrationWidget::networkRequest(const QByteArray &data)
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
