#pragma once

#include <QWidget>
#include "User.h"

namespace Ui {
class UserWidget;
}

class UserWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UserWidget(QWidget *parent = nullptr);
    ~UserWidget();
    void setupUI();
    User _user;
private:
    Ui::UserWidget *ui;
};

