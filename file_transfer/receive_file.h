#ifndef RECEIVE_FILE_H
#define RECEIVE_FILE_H

#include <QObject>
#include <QWidget>
#include<QRunnable>
#include<QDataStream>
#include<QTcpSocket>
#include<QEventLoop>
#include <QFile>

class Receive_file : public QObject ,public QRunnable
{
    Q_OBJECT
public:
    explicit Receive_file(QString file_storage_address,qintptr handle);//服务器接收文件
    Receive_file(QByteArray file_information,QString file_storage_address,QString ip_address);//客户端接收文件
    ~Receive_file();
    void run() override;
private:
    qintptr handle;//通信描述符
    QTcpSocket *socket;
    QString file_storage_address="";//文件存储地址
    QString ip_address="";
    QDataStream* stream;
    QByteArray file_information="";//文件信息
    quint64 filename_size=0;
    QByteArray filename="";
    QFile file;
    quint64 file_size=0;
    quint64 received_file_zie=0;//未接收的文件的大小
    bool flag=false;//true为服务器接收文件，false客户端接收文件

signals:
   void end();
private slots:
    void recework();
};

#endif // RECEIVE_FILE_H
