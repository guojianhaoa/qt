#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
 客户端发送的数据格式
 0 注册 账号+密码+昵称
 1 登录 账号+密码
 2 存储socket 账号
 3 发送消息  账号+目标账号+消息
 4 添加好友  账号+目标账号+昵称
 服务端发送数据的格式
 0 回应注册 true Or false
 1 回应登录 true+账号+昵称+所有（好友账号+昵称）Or false
 3 转发消息消息  消息发送方账号+消息 Or 3false
 4 回应添加好友  目标账号+1 or 2(1表示发起添加好友的用户，2反之)+昵称 false1 Or false2(好友已存在)

 sqlite设计
 每个用户一个数据库文件
 数据库文件用来存储聊天记录
 用户和不同的目标用户的聊天记录都会创建一个表，表名为目标用户的id
 聊天记录表(chat_records)字段为 user_account text PRIMARY KEY 主键 表示聊天对象的账号，也是聊天记录数据表的表名
                    user_name text not null 表示聊天对象名字
                    visibility bool not null 表示是否显示和聊天对象的聊天记录
                    unread_message int not null 表示未读消息数
                    time int 表示和聊天对象最近的聊天时间
 聊天记录数据表 字段为 id INTEGER PRIMARY KEY 自增主键
                    type char(1) not null 表示数据类型 0==文字 ,1==图片,2==文件,3==已删除
                    left_right bool not null 表示消息在聊天窗口里在左边还是右边，false表示左
                    message text not null 表示聊天信息，图片和文件已存储的地址表示，为NULL则说明未下载

*/
MainWindow::MainWindow(QStringList list,QTcpSocket *socket,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{    //设置调用colse函数后不隐藏而是销毁窗口
    this->setAttribute(Qt::WA_DeleteOnClose);
    //
    ui->setupUi(this);
    //获取登录界面传过来的socket
    this->socket=socket;
    //取消socket和其它任何对象的关联关系,即取消socket在登录界面的连接
    disconnect(socket,0,0,0);
    //处理服务端转发的信息
    connect(this->socket,&QTcpSocket::readyRead,[=]{
        qDebug()<<"服务器有数据发送："<<endl;
        QString gserver_information=this->receive_data();
        qDebug()<<gserver_information<<endl;
        if(gserver_information!="F"){
           this->processingData(gserver_information);//将获取的数据传给processingData函数，处理数据
        }
    });
    //设置数据流操作的对象数组send_data,参数二描述如何使用设备。
   // this->send_stream=new QDataStream(&this->send_data,QIODevice::ReadWrite);
    //设置版本
    //this->send_stream->setVersion(QDataStream::Qt_5_14);
    //设置数据流操作的对象socket
    this->receive_stream=new QDataStream(socket);
    //设置版本
    //this->send_stream->setVersion(QDataStream::Qt_5_14);
    //设置界面
    ui->listWidget_2->setFixedWidth(this->width()-19);
    this->m_data_size=0;
    //获取服务器端发送的好友信息和自身账号昵称
    this->account=list[0].mid(5,8);
    this->setWindowTitle(account);
    this->name=list[0].mid(13);
    //初始化化chat_history<QString QStringList*>
//    for(int i=1;i<list.length();i++){
//        QStringList* l=new QStringList;
//        qDebug()<<"this->chat_history.insert:"<<list.value(i).mid(0,8)<<endl;
//        this->chat_history.insert(list.value(i).mid(0,8),l);
//    }
    //开启本地数据库
    this->sqlite=new mySqlite(this->account);
      //获取需要显示的聊天记录消息表
//    QStringList field_chat_records;
//    field_chat_records.append("user_account"); //查询的字段
//    field_chat_records.append("");
//    field_chat_records.append("");
//    field_chat_records.append("");
    QStringList l=sqlite->select_table("chat_records","user_account,user_name,unread_message,time","visibility=true or visibility=1 ORDER BY time desc");
    //set.insert()
    //如果表不存在则创建聊天记录表，聊天记录信息表
    if(l[0]=="false"){
       sqlite->create_table("chat_records","user_account text PRIMARY KEY,user_name text not null,visibility bool not null,unread_message int not null,time int");
       for(int i=1;i<list.length();i++){
           sqlite->create_table(list[i].mid(0,8));       
           sqlite->insert("chat_records",QString("'%1','%2',0,0,0").arg(list[i].mid(0,8)).arg(list[i].mid(8)));
       }
    }else{
        //l为聊天记录表的内容,内容以-分隔
       qDebug()<<l<<endl;
       for(int i=1;i<l.length();i++){
          //QStringList* data=new QStringList;
          QStringList field;//字段 user_account user_name unread_message time
          field=l.value(i).split("-",Qt::SkipEmptyParts);
          //int length=-1;
//          for(int j=0;j<l.value(i).length();j++){
//              if((l[i])[j]=="-"){
//                  length++;
//                  field.append("");
//              }else{
//                  field[length].append((l[i])[j]);
//              }
//          }
          mesage_widget* m=this->show_mesage_widget(field);//创建消息
          this->m_message.insert(field[0],m);//存储消息控件便于管理
          //this->m_message.insert()
       }
    }
    //检查聊天记录表内容是否完整
    QSet<QString> set;
    QStringList s_list;
    s_list=sqlite->select_table("chat_records","user_account");//获取chat_records里的所有的聊天记录消息表表名
    if(s_list[0]=="true"){
        for(int i=1;i<s_list.length();i++){
            set.insert(s_list[i].mid(1));
        }
    }else{
        qDebug()<<"s_list: 查询失败"<<endl;
    }
    for(int i=1;i<list.length();i++){//list 为所有的好友的账号和名字
        if(!set.contains(list[i].mid(8))){//如果不存在
            sqlite->create_table(list[i].mid(0,8));
            sqlite->insert("chat_records",QString("'%1','%2',0,0,0").arg(list[i].mid(0,8)).arg(list[i].mid(8)));
        }
    }
    //往好友列表添加好友
    for (int i=1;i<list.length();i++) {
        QListWidgetItem *item=new QListWidgetItem(list[i],ui->listWidget);
    };
    //添加好友
    connect(ui->pushButton,&QPushButton::clicked,[=]{
        //socket->write(QString("4%1%2%3").arg(this->account).arg(ui->lineEdit->text()).arg(this->name).toUtf8());
        this->send_packets(QString("4%1%2%3").arg(this->account).arg(ui->lineEdit->text()).arg(this->name).toUtf8());
    });

    connect(ui->listWidget,&QListWidget::itemClicked,this,[this](QListWidgetItem* item){
        show_mainwidow(item->text().mid(8),item->text().mid(0,8));
    });

    connect(ui->listWidget_2,&QListWidget::itemClicked,this,[this](QListWidgetItem *item){
        QString str=item->data(Qt::UserRole).toString();
        qDebug()<<"点击消息列表事件"<<str<<endl;
        show_mainwidow(str.mid(8),str.mid(0,8));

    });
}

MainWindow::~MainWindow()
{
    for (auto value : m_chat) {
        delete value;
    }
    for(auto value:m_message){
        delete value;
    }
    delete socket;
    delete ui;
}
void MainWindow:: processingData(const QString& gserver_information){//处理服务器消息
        //QString str=s->readAll();
     qDebug()<<"mainwindow接收信息:"<<gserver_information<<endl;
     char c=gserver_information.at(0).toLatin1();
     switch (c) {
         case '3':{
              //将私聊信息发送到对应窗口
            QString str3=gserver_information.mid(1,8);
            bool flag=false;//用于标识是否需要显示未读消息数
             // QString message=this->account+str.mid(9);
             int id=sqlite->insert_chat_history(str3,'0',false,gserver_information.mid(9));
             if(id==-1){
                 qDebug()<<"聊天记录插入失败"<<endl;
             }else{
                 if(this->m_chat.contains(str3)){//判断是否有和str3(私聊消息的账户)的聊天界面，若有则不需增加未读消息
                     emit m_chat.value(str3)->receive_chatdata(gserver_information.mid(9));
                 }else{
                     flag=true;
                     QString update_fied="visibility=1,unread_message=unread_message+1";//修改的字段
                     QString update_where=QString("user_account=='%1'").arg(str3);//修改条件
                     if(sqlite->update("chat_records",update_fied,update_where))qDebug()<<"更新未读消息数失败"<<endl;
                     else qDebug()<<"更新未读消息数成功"<<endl;
                     //修改chat_records,未读消息数+1
                 }
                 if(!m_message.contains(str3)){//判断是否有对应的消息控件，没有则创建
                     //获取创建消息控件所需要的数据
                     QStringList n=this->sqlite->select_table("chat_records","user_account,user_name,unread_message,time",QString("user_account='%1'").arg(str3));
                     QStringList field;
                     if(n[0]=="true"){
                         field=n[1].split("-",Qt::SkipEmptyParts);//数据只有一条
                         mesage_widget* m=this->show_mesage_widget(field);//创建消息控件
                         m_message.insert(str3,m);//存储消息控件
                     }else{
                         qDebug()<<"创建消息控件时查询失败"<<endl;
                     }

                 }else{
                     if(flag)m_message.value(str3)->update_unread_message(1);//没有对应的聊天界面的情况
                     else m_message.value(str3)->update_unread_message(-1);

                     QString message=gserver_information.mid(9);

                     m_message.value(str3)->setmessage(message);//更新一下消息控件里显示的最后一天消息
                 }

//                 else{
//                    if(this->chat_history.contains(str3)){
//                       this->chat_history.value(str3)->append(id+"-0-false-"+gserver_information.mid(9));
//                       //chatmessage.insert(str3,l);
//                    }
//                    else{
//                       QStringList* history=new QStringList;
//                       history->append(id+"-0-false-"+gserver_information.mid(9));
//                       chat_history.insert(str3,history);
//                    }
//                 }
             }
         }
         break;
         case '4'://添加好友
            if(gserver_information.length()!=7){
                 QListWidgetItem *item=new QListWidgetItem(gserver_information.mid(2),ui->listWidget);
//                 QString name;
//                 bool flag=false;
//                 if(gserver_information.at(1)=='1')flag=true;//自己是添加好友请求的发送方
//                 if(flag){
//                    name=this->name;
//                 }else{
//                     name=gserver_information.mid(10);
//                 }
                 QString data=QString("%1 %2 0 0 0").arg(gserver_information.mid(2,8)).arg(name);//往聊天记录表添加一条数据
                 if(sqlite->insert("chat_records",data)){//创建聊天记录信息表成功

                    if(sqlite->create_table(gserver_information.mid(2,8))){
                        QStringList fied;
                         fied.append(gserver_information.mid(2,8));//账号
                         fied.append(gserver_information.mid(10));//姓名
                         fied.append("0");//未读消息数
                         fied.append("0");//时间
                         this->show_mesage_widget(fied);//显示消息控件
                    }else{
                         qDebug()<<"创建聊天记录信息表失败:"<<gserver_information<<endl;
                    }
                 }else{
                        qDebug()<<"创建聊天记录表失败:"<<gserver_information<<endl;
                 }
            }
         break;
        }
}

//void MainWindow:: click_on_a_friend_event(const QListWidgetItem* const item){//好友点击事件
//    qDebug()<<"调用了click_on_a_friend_event:"<<item->text().mid(8)<<endl;
//    if(m_chat.contains(item->text().mid(0,8))){
//        m_chat.value(item->text().mid(0,8))->show();
//    }
//    else{//创建聊天窗口
//        chat_window *chat=new chat_window(item->text().mid(8),this->name,item->text().mid(0,8));
//        // chat->setParent(this);
//        m_chat.insert(item->text().mid(0,8),chat);
//        // connect(chat,&ChatWaindow::)
//        chat->show();
//        //将聊天消息转发到服务器
//        connect(chat,&chat_window::Send_chatdata,[this](QString chatdata){
//            //ui->textEdit->append(chat_message+"\n");//查看消息是否正常转发
//            //this->socket->write(QString("3%1%2").arg(this->account).arg(chat_message).toUtf8());
//            this->send_packets(QString("3%1%2").arg(this->account).arg(chatdata).toUtf8());
//        });

//    }
//}
void MainWindow::closeEvent(QCloseEvent *e){//重写关闭窗口事件
   // socket->write("消息发送成功");
    QString str=QString("5%1").arg(this->account);
    qDebug()<<"closeEvent:"<<str<<endl;
    //this->socket->write(str.toUtf8());
    send_packets(str.toUtf8());
    this->hide();
    QElapsedTimer t;
    t.start();
    while(t.elapsed()<1000) {
      //qDebug()<<fclose<<endl;
    };
    qDebug()<<"closeEvent:"<<close()<<endl;
    //close();
}

void MainWindow::send_packets(const QByteArray &data)//向服务器发送数据包
{
      QDataStream send_stream(&this->send_data,QIODevice::ReadWrite);
      quint64 data_size=(quint64)data.size();
      send_stream<<data_size;//将数据放入码流，首先放入数据的长度data_size，quint64为8字节的长度，后面就是数据
      send_stream.writeRawData(data.data(),data.length());
      qDebug()<<"向服务器发送数据:"<<send_data<<endl;
      this->socket->write(send_data);
      send_data="";//清除数据
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    ui->widget_2->resize(this->width(),this->height()-15);//让widget_2和mainwidow一样大
    ui->listWidget_2->setFixedWidth(ui->widget_2->width()-15);

    for(int i=0;i<ui->listWidget_2->count();i++){
        QListWidgetItem *item=ui->listWidget_2->item(i);
        QWidget* w=ui->listWidget_2->itemWidget(item);
        w->setFixedWidth(ui->widget_2->width()-20);
        item->setSizeHint(QSize(w->width(),w->height()));

        //item->
    }
}
QString MainWindow::receive_data()
{

   if(m_data_size==0){
       if(socket->bytesAvailable()<(qint64)sizeof(m_data_size))return "F";
       *receive_stream>>m_data_size;
       qDebug()<<"MainW:data_size大小为"<<m_data_size<<endl;
   }
   if(socket->bytesAvailable()<(qint64)m_data_size){
       return "F";
   }
   QByteArray bytearry;
   bytearry.resize(m_data_size);
   receive_stream->readRawData(bytearry.data(),m_data_size);
   qDebug()<<":"<<bytearry<<endl;
   m_data_size=0;
   return  QString::fromUtf8(bytearry);
}

chat_window* MainWindow::show_mainwidow(QString name, QString account)
{
    chat_window* chat;//聊天窗口
    QStringList history;//聊天记录
//    if(this->chat_history.value(account)->length()>0){
//       history+=*chat_history.value(account);
//       qDebug()<<"history"<<history<<endl;
//       qDebug()<<"chat_history.vale(account)"<<*chat_history.value(account)<<endl;
//       chat_history.value(account)->clear();
//    }

    if(!m_chat.value(account)){//若对应的聊天窗口不存在
        int x=this->sqlite->data_quantity(account);//获取聊天记录总条数
        history=this->sqlite->select_chat_history(account,50,x-50+1);//获取聊天记录
        if(history.length()>0){
            chat=new chat_window(name,this->name,account,&history);
        }else{
           chat=new chat_window(name,this->name,account);
        }
        chat->setWindowTitle(name);
        connect(chat,&chat_window::Send_chatdata,[this](QString chatdata){
            //ui->textEdit->append(chat_message+"\n");//查看消息是否正常转发
            //this->socket->write(QString("3%1%2").arg(this->account).arg(chat_message).toUtf8());
            this->send_packets(QString("3%1%2").arg(this->account).arg(chatdata).toUtf8());
            sqlite->insert_chat_history(chatdata.mid(0,8),'0',true,chatdata.mid(8));
            QString update_fied="visibility=1,unread_message=0";//修改的字段
            QString update_where=QString("user_account='%1'").arg(chatdata.mid(0,8));//修改条件
            if(!sqlite->update("chat_records",update_fied,update_where))qDebug()<<"show_mainwindow修改失败"<<endl;//修改chat_records,将未读消息数改为0
//            if(!m_message.contains(chatdata.mid(0,8))){//判断是否有对应的消息控件，没有则创建
//                //获取创建消息控件所需要的数据
//                QStringList n=this->sqlite->select_table("chat_records","user_account,user_name,unread_message,time",QString("user_account='%1'").arg(chatdata.mid(0,8)));
//                QStringList field;
//                if(n[0]=="true"){
//                    qDebug()<<"1:"<<field<<endl;
//                    field=n[1].split("-",Qt::SkipEmptyParts);//数据只有一条
//                    qDebug()<<"2"<<endl;
//                    mesage_widget* m=this->show_mesage_widget(field);//创建消息控件
//                    qDebug()<<"3"<<endl;
//                    m_message.insert(chatdata.mid(0,8),m);//存储消息控件
//                }else{
//                    qDebug()<<"创建消息控件时查询失败"<<endl;
//                }

//            }else{
//                QString message=chatdata.mid(8);
//                m_message.value(chatdata.mid(0,8))->text_overflow(false,message);
//            }
            QString message=chatdata.mid(8);
            m_message.value(chatdata.mid(0,8))->setmessage(message);
        });       
        m_chat.insert(account,chat);//存入m_chat
        connect(chat,&chat_window::close_chat_window,this,[this](QString account){//聊天窗口关闭时
            //this->m_chat.erase(account);
            QHash<QString,chat_window*>::iterator it=this->m_chat.find(account);//获取迭代器
            this->m_chat.erase(it);
        });
        chat->show();
    }else{
        m_chat.value(account)->show();
    }
    if(!m_message.contains(account)){//判断是否有对应的消息控件，没有则创建
        //获取创建消息控件所需要的数据
        QStringList n=this->sqlite->select_table("chat_records","user_account,user_name,unread_message,time",QString("user_account='%1'").arg(account));
        QStringList field;
        if(n[0]=="true"){
            qDebug()<<"1:"<<field<<endl;
            field=n[1].split("-",Qt::SkipEmptyParts);//数据只有一条
            qDebug()<<"2"<<endl;
            mesage_widget* m=this->show_mesage_widget(field);//创建消息控件
            qDebug()<<"3"<<endl;
            m_message.insert(account,m);//存储消息控件
        }else{
            qDebug()<<"创建消息控件时查询失败"<<endl;
        }
    }else{
        m_message.value(account)->update_unread_message(-1);
    }
    return chat;
}

mesage_widget* MainWindow::show_mesage_widget(QStringList& field)
{
    int x=sqlite->data_quantity(field[0]);//获取聊天记录条数
    qDebug()<<"x:"<<x<<endl;
    QStringList next_history=sqlite->select_chat_history(field[0],1,x);//获取最后一条聊天记录
    QString message=next_history.value(0).section("-",3,-1);//取出聊天消息
    //this->chat_history.insert(field[0],data);//纯存储聊天记录
    QListWidgetItem *item=new QListWidgetItem();

    item->setData(Qt::UserRole,field[0]+field[1]);
    //创建消息
    QString name=field[1];
    mesage_widget *m=new mesage_widget(name,message);
    ui->listWidget_2->addItem(item);
    m->setFixedWidth(ui->listWidget_2->width());
    m->update_unread_message(field[2].toInt());//更新未读消息数
    item->setSizeHint(m->size());
    ui->listWidget_2->setItemWidget(item,m);


    //点击好友列表事件
    return m;
}
