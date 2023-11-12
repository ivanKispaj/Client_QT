#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "User.h"
#include "message.h"
#include "tcpclient.h"
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    enum WidgetType
    {
        user,message
    };

signals:
    void destruct(int index);
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createNewClient();
    int _windowIndex{0};
    static QList<MainWindow*> _clientWindow;
private slots:

    /**
   * @brief on_messageTextEdit_textChanged
   * Text change event in text editor
   */
    void on_messageTextEdit_textChanged();
    void on_sendMessageButton_clicked();
    void on_goToPublicChat_clicked();
    void userIsLogined(const User &user);
    void authentificate();
    void on_actionOpen_owner_client_triggered();
    void updateUI(const QByteArray &data);
    void onItemClicked(const QModelIndex &index);
    void updateSizeItem(int height);
    void applyResizeWindow();
    void loadAppData();


    void on_actionClose_this_client_triggered();

private:
    Ui::MainWindow *ui;
    TCPClient *_network = new TCPClient();
    User _user;
    bool _isPrivateChatActive{false};
    int _recipientId{0};
    QVector<User> _users;
    QVector<Message> _messages;
    QVector<Message> _p_Messages;

    void showEvent(QShowEvent *event) override;
    void updateChatStateTypeUI();
    QString getUserNameId(int id);
protected:
    void resizeEvent(QResizeEvent *event) override;
    void appendWidgetToListWidget(const WidgetType type, const QJsonObject &object);
    void clearItemWidgetOn(WidgetType type);

};
#endif // MAINWINDOW_H
