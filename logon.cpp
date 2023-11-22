#include "logon.h"
#include "ui_logon.h"
/*
 客户端发送的数据格式
 0 注册 账号+密码+昵称
 1 登录 账号+密码
 2 存储socket 账号
 3 发送消息  账号+目标账号+消息
 4 添加好友  目标账号+账号+昵称
 5 退出  false Or true+账号
 服务端发送数据的格式
 0 回应注册 true Or false
 1 回应登录 true+所有（好友账号+昵称）Or false
 3 好友消息  消息发送方账号+消息
 4 回应添加好友  true Or false Or false(好友已存在)
*/
LogOn::LogOn(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LogOn)
{   //设置调用colse函数后不隐藏而是销毁窗口
    this->setAttribute(Qt::WA_DeleteOnClose);
    //账号密码框设置
    ui->setupUi(this);
    acconticon=new QAction(ui->lineEdit);
    acconticon->setIcon(QIcon(":/icon/account2.png"));
    //acconticon->
    ui->lineEdit->addAction(acconticon,QLineEdit::LeadingPosition);//在账号输入框添加图标
    passowordicon=new QAction(ui->lineEdit_2);
    passowordicon->setIcon(QIcon(":/icon/password2.png"));
    ui->lineEdit_2->addAction(passowordicon,QLineEdit::LeadingPosition);//在密码输入框添加图标
    //添加事件过滤器
    ui->lineEdit_2->installEventFilter(this);
    ui->lineEdit->installEventFilter(this);
    ui->regster->installEventFilter(this);
    //动态背景设置
    gif=new QMovie(":/icon/Blue blur color abstract.gif");
    gif->setParent(this);
    gif->setScaledSize(QSize(531,170));
    ui->label->setMovie(gif);
    gif->start();
    //设置图标
    ui->setUp->setIcon(QIcon(":/icon/setUp.png"));
    //设置数据流操作的对象数组send_data,参数二描述如何使用设备。
    this->send_stream=new QDataStream(&this->send_data,QIODevice::ReadWrite);
    //设置版本
    this->send_stream->setVersion(QDataStream::Qt_5_14);
    //
    this->data_size=0;
    //设置版本
    //this->send_stream->setVersion(QDataStream::Qt_5_14);
    //连接服务器
    socket=new QTcpSocket();
    socket->connectToHost(ui->severip->text(),1500);
    //设置数据流操作的socket
    this->receive_stream=new QDataStream(socket);

    //注册账号
    connect(ui->regster,&QPushButton::clicked,[=](){
        Regster *reg=new Regster(socket);
        this->hide();
        reg->show();
        connect(this,&LogOn::regster,reg,&Regster::regster);
        //接收注册页面返回信号
        connect(reg,&Regster::breaklogo,[=](){
            reg->close();
            this->show();
        });
    });
    //登录设置
    connect(ui->logon,&QPushButton::clicked,[=]{
        //socket->connectToHost(ui->severip->text(),1500);
        QString str=QString("1%1%2").arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text());
        this->send_packets(str.toUtf8());
    });
    //处理注册和登录
    connect(socket,&QTcpSocket::readyRead,[=](){
        qDebug()<<"服务器有数据发送"<<endl;
        QString gserver_information=this->receive_data();
        if(gserver_information!="F"){
           this->processingData(gserver_information);
        }
    });

}

LogOn::~LogOn()
{
    delete ui;
}

bool LogOn::eventFilter(QObject *watched, QEvent *event)//处理密码框焦点事件，更换图片
{
    if(watched==ui->lineEdit_2){
        if(event->type()==QEvent::FocusOut){//失去焦点
            passowordicon->setIcon(QIcon(":/icon/password2.png"));
        }
        else if(event->type()==QEvent::FocusIn){//获得焦点
            passowordicon->setIcon(QIcon(":/icon/password.png"));
        }
    }
    else if(watched==ui->lineEdit){
        if(event->type()==QEvent::FocusOut){//失去焦点
            acconticon->setIcon(QIcon(":/icon/account.png"));
        }
        else if(event->type()==QEvent::FocusIn){//获得焦点
            acconticon->setIcon(QIcon(":/icon/account2.png"));
        }
    }
    else if(watched==ui->regster){
        if(event->type()==QEvent::Enter){
            ui->regster->setStyleSheet("QPushButton{color: rgb(7, 220, 245);}");
        }
        else if(event->type()==QEvent::Leave){
            ui->regster->setStyleSheet("QPushButton{color: rgb(176, 176, 176);}");
        }
    }
    return QWidget::eventFilter(watched,event);
}

void LogOn::closeEvent(QCloseEvent *e)
{

  if(this->account==""){
      socket->write("5false");
      this->hide();
      QElapsedTimer t;
      t.start();
      while (t.elapsed()<1000) {
         // qDebug()<<t.elapsed()<<endl;
      }
   }else{

  }

  this->close();
}

void LogOn::send_packets(const QByteArray &data)
{

    quint64 size=(quint64)data.size();
    send_data.resize(size+sizeof(size));
    *this->send_stream<<size;//将数据放入码流，首先放入数据的长度data_size，quint64为8字节的长度，后面就是数据
    this->send_stream->writeRawData(data.data(),data.length());
    this->socket->write(send_data);
    qDebug()<<"logon:"<<send_data<<endl;
    send_data="";//清除数据
}
QString LogOn::receive_data()
{
     //qDebug()<<"logon:"<<socket->readAll()<<endl;
    qDebug()<<"1"<<endl;
    if(data_size==0){

        if(socket->bytesAvailable()<(qint64)sizeof(data_size))return "F";
        qDebug()<<"2"<<endl;
        *receive_stream>>data_size;
        qDebug()<<"logon:data_size大小为"<<data_size<<endl;
    }
    qDebug()<<"3"<<endl;
    if(socket->bytesAvailable()<(qint64)data_size){
        return "F";
    }
    QByteArray bytearry;
    bytearry.resize(data_size);
    receive_stream->readRawData(bytearry.data(),data_size);
    qDebug()<<":"<<bytearry<<endl;
    data_size=0;
    return  QString::fromUtf8(bytearry);
}
void LogOn::processingData(const QString &server_information)
{

    qDebug()<<"logon:"<<server_information<<endl;
    if(server_information[0]=="0"){
        //触发注册信号
        if(server_information.mid(1)=="true"){
            bregster=true;
            emit regster(true);
        }
        else{
            bregster=false;
            emit regster(false);
        }
    }
    else if(server_information[0]=="1"){
        if(server_information.mid(1,4)=="true"){
            QStringList list;
            QString str;
           for(int i=0;i<server_information.length();i++){
               if(server_information.at(i)!='-'){
                   str.append(server_information.at(i));
               }
               else{
                   list.append(str);
                   str="";
               }
           }
           this->account=server_information.mid(5,8);
           list.append(str);
           MainWindow *m=new MainWindow(list,socket);
           m->show();
           this->close();
        }
    }
}
