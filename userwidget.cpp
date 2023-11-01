#include "userwidget.h"
#include "./ui_userwidget.h"

UserWidget::UserWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::UserWidget)
{
    ui->setupUi(this);
}

UserWidget::~UserWidget() { delete ui; }

void UserWidget::setupUI()
{
    ui->wUserLogin->setText(_user.getUserLogin());
    ui->wUserName->setText(_user.getUserName());
    ui->groupBox->setTitle("id: " + QString::number(_user.getId()));
    ui->isAdminUser->setEnabled(false);
    ui->isAdminLableForCheck->hide();
    ui->isAdminUser->hide();

    if (_user.isAdmin())
    {
        ui->isAdminUser->setHidden(false);
        ui->isAdminLableForCheck->setHidden(false);
        ui->isAdminUser->setCheckState(Qt::CheckState::Checked);
        ui->isAdminLableForCheck->setText(tr("is admin"));
    }
}
