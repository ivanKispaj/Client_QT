#include "messagewidget.h"
#include "ui_messagewidget.h"
#include <QTextOption>

MessageWidget::MessageWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MessageWidget)
{
    ui->setupUi(this);
}

MessageWidget::~MessageWidget()
{
    delete ui;
}

void MessageWidget::setupUI()
{
    ui->groupBox->setTitle("id: " + QString::number(_message.getId()));
    ui->messageTextEdit->setText(_message.getMessage());
    ui->authorNamelable->setText(tr("Author message: ") + _authorName);
    setDataUI(_message.getDate());

}

void MessageWidget::resizeUI(QSize size)
{
    this->resize(size);
}

void MessageWidget::setDataUI(const QDateTime &date)
{
    QString formattedDate = date.toString("dd.MM.yyyy hh:mm");
    ui->dateLable->setText(formattedDate);
}


void MessageWidget::on_messageTextEdit_textChanged()
{
    // Получение высоты QTextDocument
    int documentHeight = ui->messageTextEdit->document()->size().height();
    emit updateSize(documentHeight);
}

