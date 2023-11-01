#include "authorization.h"
#include "ui_authorization.h"

Authorization::Authorization(TCPClient *network ,QWidget *parent)
    : _network(network), QDialog(parent)
    , ui(new Ui::Authorization)
{
    ui->setupUi(this);
    ui->loginWidget->setNetworkService(_network);
    ui->registerWidget->setNetworkService(network);

    connect(ui->loginWidget,
            &LoginWidget::registrationRequesterd,
            this,
            &Authorization::setRegistrationForm);
    connect(ui->registerWidget,
            &RegistrationWidget::loginRequesterd,
            this,
            &Authorization::setLoginForm);
    connect(ui->loginWidget, &LoginWidget::accepted, this, &Authorization::onLoginedUser);
    connect(ui->registerWidget, &RegistrationWidget::accepted, this, &Authorization::onLoginedUser);
    connect(ui->loginWidget, &LoginWidget::rejected, this, &Authorization::rejected);
    connect(ui->registerWidget, &RegistrationWidget::rejected, this, &Authorization::rejected);
    setLoginForm();
}

Authorization::~Authorization() { delete ui; }

void Authorization::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(0);
    this->setWindowTitle(tr("Login form"));
}

void Authorization::setRegistrationForm()
{
    ui->stackedWidget->setCurrentIndex(1);
    this->setWindowTitle(tr("Registration form"));
}

void Authorization::onLoginedUser(const User &user)
{
    emit onLogined(user);
    QDialog::accept();
}

void Authorization::rejected() { QDialog::reject(); }
