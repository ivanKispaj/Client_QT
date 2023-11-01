//
// mainwindow.cpp
// 2023-03-11
// Ivan Konishchev
//

#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "authorization.h"
#include <QShowEvent>
#include <QtCore>
#include "userwidget.h"
#include "message.h"
#include "messagewidget.h"
#include <QSizePolicy>
#include <QMessageBox>

QList MainWindow::_clientWindow = QList<MainWindow*>();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("Chat client"));
    // connects
    connect(_network, &TCPClient::responseReceived, this, &MainWindow::updateUI);
    connect(ui->usersListWidget, &QListView::clicked, this, &MainWindow::onItemClicked);
    ui->verticalLayout_3->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
    ui->messagesListWidget->setSelectionMode(QAbstractItemView::NoSelection);
    ui->messageTextEdit->setPlaceholderText(tr("Enter your message...."));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete _network;
}


void MainWindow::createNewClient()
{
    MainWindow *m = new MainWindow();
    m->setAttribute(Qt::WA_DeleteOnClose);
    m->show();
    qDebug() << "новое окно открыто";
}

void MainWindow::on_messageTextEdit_textChanged()
{
    // Получение высоты QTextDocument
    int documentHeight = ui->messageTextEdit->document()->size().height();
    // resize textEdit height
    ui->messageTextEdit->setMinimumHeight(documentHeight);
}

void MainWindow::on_sendMessageButton_clicked()
{

    Message message;
    message.setMessage(ui->messageTextEdit->toPlainText());
    message.setRecipientID(_recipientId);
    message.setAuthorID(_user.getId());
    message.setIsPrivateChat(_isPrivateChatActive);
    QJsonObject obj;
    obj["message"] = message.encodeToJson();
    _network->sendResponse(TCPClient::ADD_MESSAGE,obj);
    ui->messageTextEdit->clear();
}

void MainWindow::on_goToPublicChat_clicked()
{
    _isPrivateChatActive = false;
    _recipientId = 0;
    _messages.clear();
    updateChatStateTypeUI();
    ui->usersListWidget->clearSelection();
}

void MainWindow::userIsLogined(const User &user)
{
    ui->userDataDescription->setText(user.getUserName());
    loadAppData();
    this->setWindowTitle(tr("Chat client: ") + user.getUserLogin());
    _user = user;
    setTimerForUpdate();
}

void MainWindow::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    authentificate();
}

void MainWindow::updateChatStateTypeUI()
{
    if (!_isPrivateChatActive)
    {
        ui->chatTypeLable->setText(tr("Public chat"));
        ui->goToPublicChat->hide();
        _network->sendResponse(TCPClient::GET_ALL_PUBLIC_MESSAGES);
    } else
    {
        ui->chatTypeLable->setText(tr("Private chat"));
        ui->goToPublicChat->setHidden(false);
        _network->sendResponse(TCPClient::GET_ALL_PRIVATE_MESSAGE);
    }
}

QString MainWindow::getUserNameId(int id)
{
    for(const User& user : _users)
    {
        if (user.getId() == id)
        {
            return user.getUserName();
        }
    }
    return "";
}

void MainWindow::setTimerForUpdate()
{
    QTimer *timer = new QTimer();

    // Установка интервала в 5 секунд
    timer->setInterval(2500); // 5000 миллисекунд = 5 секунд
    // Подключение функции к сигналу таймера
    connect(timer, &QTimer::timeout,this, &MainWindow::loadAppData);
    // Запуск таймера
    timer->start();
}

void MainWindow::loadAppData()
{
    _network->sendResponse(TCPClient::GET_ALL_USERS);
    updateChatStateTypeUI();
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QTimer::singleShot(0, this, SLOT(applyResizeWindow()));
}

void MainWindow::appendWidgetToListWidget(const WidgetType type, const QJsonObject &object)
{
    QListWidgetItem* item;
    switch (type) {
    case user:
    {
        User user;
        user.decodeFromJson(object);
        _users.append(user);
        if (user.getId() != _user.getId())
        {
            UserWidget *uWidget = new UserWidget;
            uWidget->_user = user;
            uWidget->setupUI();
            item = new QListWidgetItem(ui->usersListWidget);
            item->setSizeHint(QSize(150, 80));

            QSizePolicy sizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
            uWidget->setSizePolicy(sizePolicy);
            ui->usersListWidget->setItemWidget(item, uWidget);
        }
    }
    break;

    case message:
    {
        QSize size = ui->messagesListWidget->size();
        Message message;
        message.decodeFromJson(object);
        if (message.isPrivateChat())
        {
            _p_Messages.append(message);
        } else
        {
            _messages.append(message);
        }

        if ((message.isPrivateChat() && (message.getAuthorID() == _recipientId) &&
             message.isPrivateChat() && (message.getRecipientID() == _user.getId())) ||
            (message.isPrivateChat() && (message.getAuthorID() == _user.getId()) &&
             message.isPrivateChat() && (message.getRecipientID() == _recipientId)) ||
            !message.isPrivateChat())
        {
            item = new QListWidgetItem(ui->messagesListWidget);
            MessageWidget *mWidget = new MessageWidget;
            mWidget->_message = message;
            mWidget->_authorName = getUserNameId(message.getAuthorID());
            item->setSizeHint(QSize(size.width(), 88));
            QSize itemSize = item->sizeHint();
            mWidget->resizeUI(itemSize);
            connect(mWidget, &MessageWidget::updateSize, this, &MainWindow::updateSizeItem);
            ui->messagesListWidget->setItemWidget(item, mWidget);
            mWidget->setupUI();
        }

    }
    break;
    }
}

void MainWindow::clearItemWidgetOn(WidgetType type)
{
    ui->messagesListWidget->clear();
    ui->usersListWidget->clear();
}

void MainWindow::authentificate()
{
    QList<QWidget *> childWidgets = this->findChildren<QWidget *>();
    for (QWidget *widget : childWidgets)
    {
        if (widget->objectName() == "centralwidget")
        {
            widget->hide();
            break;
        }
    }
    Authorization *auth = new Authorization(_network);
    connect(auth, &Authorization::onLogined, this, &MainWindow::userIsLogined);
    auto result = auth->exec();
    delete auth;

    if (result == QDialog::Accepted)
    {
        for (QWidget *widget : childWidgets)
        {
            if (widget->objectName() == "centralwidget")
            {
                widget->setHidden(false);
                break;
            }
        }
    } else
    {
        this->close();
    }
}

void MainWindow::on_actionOpen_owner_client_triggered() { createNewClient(); }

/**
 * @brief MainWindow::updateUI
 * @param data
 *  QJsonObject value:
 *  state -> статус выполнения запроса
 *  users -> получены пользлватели
 *  messages -> получены все публичные сообщения
 *  p_messages -> получены все приватные сообщения
 *  message -> получено новое публичное сообщение
 *  p_message -> получено новое приватное сообщение
 *
 */

void MainWindow::updateUI(const QByteArray &data)
{
    QJsonObject obj = QJsonDocument::fromJson(data).object();
    if (!obj.isEmpty() && obj.contains("state"))
    {
        if (obj["state"].toBool())
        {
            if (obj.contains("users"))
            {
                QJsonArray users = obj["users"].toArray();
                if (users.count() == _users.count())
                {
                    return;
                }
                ui->usersListWidget->clear();
                for (int i = 0; i < users.size(); ++i)
                {
                    User user;
                    QJsonValue value = users.at(i);
                    if (value.isObject())
                    {
                        QJsonObject jsonObject = value.toObject();
                        appendWidgetToListWidget(WidgetType::user, jsonObject);
                    }
                }
            } else if (obj.contains("messages"))
            {
                QJsonArray messages = obj["messages"].toArray();
                if (messages.count() == _messages.count())
                {
                    return;
                }
                ui->messagesListWidget->clear();
                for(int i = 0; i < messages.count(); i++)
                {
                    Message message;
                    QJsonValue value = messages.at(i);
                    if (value.isObject())
                    {
                        QJsonObject jsonOject = value.toObject();
                        appendWidgetToListWidget(WidgetType::message, jsonOject);
                    }
                }
            } else if (obj.contains("p_messages"))
            {
                QJsonArray messages = obj["p_messages"].toArray();
                if (messages.count() == _p_Messages.count())
                {
                    return;
                }
                ui->messagesListWidget->clear();
                for(int i = 0; i < messages.count(); i++)
                {
                    Message message;
                    QJsonValue value = messages.at(i);
                    if (value.isObject())
                    {
                        QJsonObject jsonOject = value.toObject();
                        appendWidgetToListWidget(WidgetType::message, jsonOject);
                    }
                }
            } else if (obj.contains("message"))
            {
                appendWidgetToListWidget(WidgetType::message, obj["message"].toObject());
            } else if (obj.contains("p_message"))
            {
                appendWidgetToListWidget(WidgetType::message, obj["p_message"].toObject());
            }
        }
    }
    ui->usersListWidget->scrollToBottom();
    ui->messagesListWidget->scrollToBottom();
}

void MainWindow::onItemClicked(const QModelIndex &index)
{

    QListWidgetItem *item = ui->usersListWidget->itemFromIndex(index);
    UserWidget* itemWidget = qobject_cast<UserWidget*>(ui->usersListWidget->itemWidget(item));
    _recipientId = itemWidget->_user.getId();
    _isPrivateChatActive = true;
    updateChatStateTypeUI();
    ui->messagesListWidget->clear();
    _p_Messages.clear();
}

void MainWindow::updateSizeItem(int height)
{
    MessageWidget *widget = qobject_cast<MessageWidget*>(sender());
    QListWidgetItem* item{nullptr};
    for (int i = 0; i < ui->messagesListWidget->count(); ++i) {
        item = ui->messagesListWidget->item(i);
        MessageWidget* itemWidget = qobject_cast<MessageWidget*>(ui->messagesListWidget->itemWidget(item));

        if (itemWidget == widget) {
            QSize size = item->sizeHint();
            item->setSizeHint(QSize(size.width(), 88 + height));
            break;
        }
    }

}

void MainWindow::applyResizeWindow()
{

    QSize size = ui->messagesListWidget->size();
    QListWidgetItem* item{nullptr};
    for (int i = 0; i < ui->messagesListWidget->count(); ++i) {
        item = ui->messagesListWidget->item(i);
        item->setSizeHint(QSize(size.width(),item->sizeHint().height()));
        MessageWidget* itemWidget = qobject_cast<MessageWidget*>(ui->messagesListWidget->itemWidget(item));
        itemWidget->resizeUI(item->sizeHint());
        itemWidget->setupUI();
    }
}











