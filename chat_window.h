#ifndef CHAT_WINDOW_H
#define CHAT_WINDOW_H

#include <QWidget>
#include<QVector>
#include<QHash>
#include"Chat/chat_item.h"
#include"Chat/chat_item2.h"
namespace Ui {
class chat_window;
}

class chat_window : public QWidget
{
    Q_OBJECT

public:
    explicit chat_window(QString name_left,QString name_right,QString account,QStringList* chat_history=NULL,QString head_address=NULL);
    ~chat_window();
    virtual void resizeEvent(QResizeEvent *event) override;
    void load_history(QStringList *history);
    void closeEvent(QCloseEvent *event) override;//窗口关闭事件
private:
    Ui::chat_window *ui;
    int W;
    int message=0;
    QString name_right;
    QString name_left;
    QString account;
    QString head_address;
    QHash<int,chat_item*> item1;
    QHash<int,chat_item2*> item2;
signals:
    void receive_chatdata(QString chatdata);
    void Send_chatdata(QString chatdata);
    void resize_event(int width);
    void close_chat_window(QString account);
public slots:
    void add_message(QString &chatdata,bool left_right);//left_right==false表示左边的消息
};

#endif // CHAT_WINDOW_H
