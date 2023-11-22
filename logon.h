#ifndef LOGON_H
#define LOGON_H
#include<QElapsedTimer>
#include <QWidget>
#include<QAction>
#include<QMovie>
#include<QTcpSocket>
#include"regster.h"
#include"mainwindow.h"
namespace Ui {
class LogOn;
}

class LogOn : public QWidget
{
    Q_OBJECT

public:
    explicit LogOn(QWidget *parent = nullptr);
    ~LogOn();
    QString account="";
    QAction *passowordicon;
    QAction *acconticon;
    QMovie *gif;
    QTcpSocket *socket;
    bool bregster;//注册是否成功
    bool eventFilter(QObject *watched, QEvent *event);
    void closeEvent(QCloseEvent *e);//重写窗口关闭事件
    void send_packets(const QByteArray &data);
    QString receive_data();
    void processingData(const QString& server_information);
private:
    QByteArray send_data;
    QDataStream* send_stream;
    QDataStream* receive_stream;
    quint64 data_size=0;

    Ui::LogOn *ui;
signals:
    void regster(bool flag);
};

#endif // LOGON_H
