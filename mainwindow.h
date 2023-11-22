#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<QStringList>
#include<QListWidgetItem>
#include<QListWidget>
#include<QTcpSocket>
#include<QElapsedTimer>
#include<QHash>
#include<QStringList>
#include<QCloseEvent>
#include<QSet>
#include "Chat/chat_item.h"
#include "Chat/chat_item2.h"
#include "chat_window.h"
#include"mysqlite.h"
#include"mesage_widget.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QStringList list,QTcpSocket *socket,QWidget *parent = nullptr);
    ~MainWindow();
    //void click_on_a_friend_event(const QListWidgetItem* const item);

    void closeEvent(QCloseEvent *e);//重写窗口关闭事件
    void send_packets(const QByteArray& data);
    virtual void resizeEvent(QResizeEvent *event) override;
    mesage_widget* show_mesage_widget(QStringList& field);//显示消息 field包含 user_name unread_message time

private:
    QTcpSocket *socket;
    QHash<QString,chat_window*> m_chat;
    QHash<QString,mesage_widget*>m_message;
    //QHash<QString,QStringList> chatmessage;
    //QHash<QString,QStringList*> chat_history;
    Ui::MainWindow *ui;
    QString name;
    QString account="";
    QByteArray send_data;//存储向服务器发送的数据
    //QDataStream* send_stream;
    QDataStream* receive_stream;
    bool fclose=false;//判断服务器是否接收到客户端退出消息
    quint64 m_data_size;
    mySqlite* sqlite;
public slots:
   void processingData(const QString& server_information);
   QString receive_data();
   //void uread_message(int number);
   chat_window* show_mainwidow(QString name,QString account);


};
#endif // MAINWINDOW_H
