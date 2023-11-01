#pragma once

#include <QWidget>
#include "message.h"
#include <QString>

namespace Ui {
class MessageWidget;
}

class MessageWidget : public QWidget
{
    Q_OBJECT

signals:
    void updateSize(int height);
public:
    explicit MessageWidget(QWidget *parent = nullptr);
    Message _message;
    QString _authorName;
    int _authorId{0};
    ~MessageWidget();
    void setupUI();
    void resizeUI(QSize);
private slots:

    void on_messageTextEdit_textChanged();

private:
    Ui::MessageWidget *ui;
    void setDataUI(const QDateTime &date);
};

