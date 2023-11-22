#ifndef SEND_FILE_H
#define SEND_FILE_H

#include <QObject>
#include<QRunnable>
#include<QDataStream>
#include<QFile>
#include<QDateTime>
#include<QTcpSocket>

class Send_file : public QObject ,public QRunnable
{
    Q_OBJECT
public:
    explicit Send_file(QString file_address,qintptr handle=NULL);
    Send_file(QString file_address,QString ip_address);
    ~Send_file();
    void run() override;
    void setFile_address(QString file_daaress);
private:
    QString file_address;
    QString ip_address="";
    QTcpSocket* socket;
    QByteArray file_information;
    qintptr handle;
signals:

};

#endif // SEND_FILE_H
