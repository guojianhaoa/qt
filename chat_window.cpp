#include "chat_window.h"
#include "ui_chat_window.h"

chat_window::chat_window(QString name_left,QString name_right,QString account,QStringList* chat_history,QString head_address) :
    ui(new Ui::chat_window)
{
    ui->setupUi(this);
    //初始化
     this->setAttribute(Qt::WA_DeleteOnClose);//设置调用colse函数后不是隐藏界面，而是销毁界面
    this->name_left=name_left;
    this->name_right=name_right;
    this->account=account;
    W=this->width();
    connect(ui->send_data,&QPushButton::clicked,this,[this](){
       QString str_send=ui->textEdit->toPlainText();
       ui->textEdit->setText("");
       add_message(str_send,true);
       str_send.prepend(this->account);
       emit Send_chatdata(str_send);


    });
    connect(this,&chat_window::receive_chatdata,this,[this](QString chatdata){
       add_message(chatdata,false);
    });
    if(chat_history!=NULL){
         qDebug()<<"chat_histor:"<<*chat_history<<endl;
         for(int i=0;i<chat_history->length();i++){
             int type;
             QStringList fied=chat_history->value(i).split("-");
             fied.append(chat_history->value(i).section("-",0,0));//id
             fied.append(chat_history->value(i).section("-",1,1));//type
             fied.append(chat_history->value(i).section("-",2,2));//left_right
             fied.append(chat_history->value(i).section("-",3,-1));//message
             type=fied[1].toInt();
             switch (type) {
               case 0:
                   if(fied[2]=="true" || fied[2]=="1"){//右边的消息
                       add_message(fied[3],true);
                   }else{
                       add_message(fied[3],false);
                   }
               break;
               case 1:

               break;
               case 2:

               break;
             }
         }
    }
}

chat_window::~chat_window()
{
    qDebug()<<"chat_window销毁"<<endl;
    for(QHash<int,chat_item*>::const_iterator i=item1.constBegin();i!=item1.constEnd();i++){
        delete i.value();
    }
    for(QHash<int,chat_item2*>::const_iterator i=item2.constBegin();i!=item2.constEnd();i++){
        delete i.value();
    }
    delete ui;
}

void chat_window::resizeEvent(QResizeEvent *event)
{
    emit resize_event(this->width()-this->W);//触发窗口大小改变信号，参数为改变的大小；
    this->W=this->width();
}

void chat_window::load_history(QStringList *history)
{

}

void chat_window::closeEvent(QCloseEvent *event)
{
    emit this->close_chat_window(this->account);//触发信号表示关闭
}

void chat_window::add_message(QString &chatdata, bool left_right)
{

    if(!left_right){
        QListWidgetItem *item=new QListWidgetItem;
        chat_item2* c_item2=new chat_item2(this->name_left,chatdata,this->width()-18);
        ui->listWidget->addItem(item);
        c_item2->text_line_break();
        item->setSizeHint(c_item2->getSize());
        ui->listWidget->setItemWidget(item,c_item2);
        connect(this,&chat_window::resize_event,c_item2,[item,c_item2](int width){
            c_item2->setWidth(width);
            //c_item2->setFixedWidth(width);
            c_item2->text_line_break();
            item->setSizeHint(c_item2->getSize());
        });
        message++;
        item2.insert(message,c_item2);
    }else{
        QListWidgetItem *item=new QListWidgetItem;
        chat_item* c_item=new chat_item(this->name_right,chatdata,this->width()-18);
        ui->listWidget->addItem(item);
        item->setSizeHint(c_item->getSize());
        ui->listWidget->setItemWidget(item,c_item);
        connect(this,&chat_window::resize_event,c_item,[item,c_item](int width){
            c_item->setWidth(width);
            c_item->text_line_break();
            item->setSizeHint(c_item->getSize());
        });

        message++;
        item1.insert(message,c_item);
    }
}
