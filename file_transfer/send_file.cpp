#include "send_file.h"

Send_file::Send_file(QString file_address,qintptr handle)
{
    qRegisterMetaType<qintptr>("qintptr");//注册一下"qintptr"这种类型，要不然传输不过来
    this->file_address=file_address;
    this->handle=handle;
    this->setAutoDelete(true);//任务执行完后不自动销毁
}

Send_file::Send_file(QString file_address, QString ip_address)
{
    this->file_address=file_address;
    this->ip_address=ip_address;
    this->setAutoDelete(true);
}

Send_file::~Send_file()
{
    if(socket!=NULL){
       delete  socket;
       socket==NULL;
    }


}

void Send_file::run()
{
    QDataStream stream(&file_information,QIODevice::ReadWrite);
    QFile file(file_address);
    this->socket=new QTcpSocket();
    if(ip_address=="")socket->setSocketDescriptor(handle);//服务器转发
    else socket->connectToHost(ip_address,1880);//向服务器发送
    if(file_address==""){
        return;
    }
    qDebug()<<"子线程接收到的文件路径为:"<<file_address<<endl;
    if(file.open(QIODevice::ReadOnly)){
        qDebug()<<"子线程成功打开了文件"<<endl;
    }
    else{
        qDebug()<<"子线程打开文件失败"<<endl;
        return;
    }
    //获取时间
    QDateTime currentDateTime = QDateTime::currentDateTime();

    //获取原文件名
    QString name="";
    for(int i=file_address.length()-1;i>=0;i--){
        if(file_address[i]=="/")break;
        name.prepend(file_address[i]);
    }
    // 获取带毫秒当前时间的字符串，并加入文件件名  文件名为 当前时间+原文件名
    name.prepend(currentDateTime.toString("yyyy-MM-dd-hh-mm-ss-zzz"));
    //写入文件名大小和文件名
    stream<<(quint64)name.size();
    stream.writeRawData(name.toUtf8(),name.size());
    qDebug()<<"图片名"<<name<<endl;
    qDebug()<<"当前读入的数据："<<file_information<<endl;
    qDebug()<<"获取图片大小"<<file.size()<<endl;
    stream<<(quint64)file.size();
    qDebug()<<"当前读入的数据："<<file_information<<endl;
    //发送文件名大小，文件名，图片大小
     //socket->write(image_byte,image_byte.size());
    //stream.writeRawData(image.readAll().data(),image.size());
    //stream.writeRawData(image.read(65535-image_byte.size()),image_byte.size());
    this->socket->write(file_information);
    this->socket->flush();//立即发送缓冲区的数据
    while(!file.atEnd()){
        qDebug()<<"未发送数据"<<this->socket->bytesToWrite()<<endl;
        if(this->socket->bytesToWrite()==0){
            this->socket->write(file.read(65535));
            this->socket->waitForBytesWritten();//立即发送缓冲区的数据
            qDebug() <<"子线程读入图片数据"<<endl;
        }

    }
    qDebug() <<"子线程已将图片数据读入byte"<<endl;
    qDebug()<<"子线程向客户端发送图片"<<endl;
    //this->socket->write(byte);

    //emit this->send();
    file.close();
}

void Send_file::setFile_address(QString file_daaress)
{
    this->file_address=file_address;
}
