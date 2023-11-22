#include "receive_file.h"

Receive_file::Receive_file(QString file_storage_address,qintptr handle)
{
    qRegisterMetaType<qintptr>("qintptr");
    //初始化文件存放地址
    this->file_storage_address=file_storage_address;
    this->handle=handle;
    this->setAutoDelete(true);
    flag=true;
}
Receive_file::Receive_file(QByteArray file_information,QString file_storage_address,QString ip_addreess){
    this->file_storage_address=file_storage_address;
    this->ip_address=ip_address;
    this->file_information=file_information;
    this->setAutoDelete(true);
}

Receive_file::~Receive_file()
{
    if(this->socket!=NULL){
        delete this->socket;
        this->socket=NULL;
    }
    if(this->stream!=NULL){
        delete this->stream;
        this->stream=NULL;
    }
}
void Receive_file::run(){
    //读取文件名大小
    qDebug()<<"------run函数调用-------"<<endl;
    QEventLoop loop;
    this->socket=new QTcpSocket();
    this->stream=new QDataStream(socket);
    if(ip_address=="")this->socket->setSocketDescriptor(this->handle);//服务器接收
    else this->socket->connectToHost(this->ip_address,1880);//客户端接受
    if(!flag){//客户端接收需告知服务器客户端的socket已建立，并上传文件信息，服务端接收后开始传输文件
        *stream<<(quint64)file_information.size();
        stream->writeRawData(file_information,file_information.size());
        this->socket->flush();   
    }
    connect(socket,&QTcpSocket::readyRead,this,&Receive_file::recework);

    loop.exec();
}

void Receive_file::recework()
{
    qDebug()<<"------开始接收文件-------"<<endl;
   if(filename_size==0){
       while (true) {
          if(socket->bytesAvailable()>=(qint64)sizeof (filename_size))break;
       }
       *stream>>filename_size;
       qDebug()<<"文件名大小"<<filename_size<<endl;
       qDebug()<<"------"<<socket->bytesAvailable()<<endl;
   }
   //读取文件名
   if(filename==""){
       while(true){
           if(socket->bytesAvailable()>=(qint64)filename_size)break;
       }
       filename.resize(filename_size);
       stream->readRawData(filename.data(),filename_size);
       //filename.prepend(file_storage_address.toUtf8());
       file_storage_address.append("/");
       filename.prepend(file_storage_address.toUtf8());
       qDebug()<<"文件名:"<<filename<<"----文件名大小:"<<filename.size()<<endl;
       qDebug()<<"------"<<socket->bytesAvailable()<<endl;
   }
   //读取图片大小
   if(file_size==0){
       while (true) {
          if(socket->bytesAvailable()>=(qint64)sizeof (this->file_size))break;
       }
       *stream>>file_size;
       received_file_zie=file_size;
       qDebug()<<"图片大小"<<file_size<<endl;
       qDebug()<<"------"<<socket->bytesAvailable()<<endl;

       //创建图片文件
       file.setFileName(filename);
       if(file.open(QIODevice::ReadWrite)){
           qDebug()<<"文件创建成功"<<endl;
       }else{
           qDebug()<<"文件创建失败"<<endl;
       }
      // this->timer.start();
   }

   //读取图片
   if(received_file_zie!=0){

       //while(received_file_zie){
           qDebug()<<"1"<<endl;
           quint64 size=(quint64)socket->bytesAvailable();
           received_file_zie-=size;
           qDebug()<<"2"<<endl;
           file.write(socket->read(size));
           //stream->readRawData(bimage.data(),size);
           qDebug()<<"3"<<endl;
          // emit reveive_image(file_size-received_file_zie,this->timer.elapsed());
           qDebug()<<"------"<<received_file_zie<<endl;


   }else{
       file.close();
       filename="";
       filename_size=0;
       file_size=0;
       emit end();
   }
   
}
