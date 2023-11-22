#include "mesage_widget.h"
#include "ui_mesage_widget.h"

mesage_widget::mesage_widget(QString &name ,QString& message,QStringList* chat_histor) :
    ui(new Ui::mesage_widget)
{
    ui->setupUi(this);
    QFont f;
    //修改字体
      f=ui->name->font();
      f.setFamily("微软雅黑");
      f.setPixelSize(15);
      ui->name->setFont(f);
      f=ui->message->font();
      f.setFamily("微软雅黑");
      f.setPixelSize(10);
      ui->message->setFont(f);
   //
      this->name=name;
      this->message=message;
      ui->head->installEventFilter(this);
      //connect(this,&mesage_widget::upate_unread_message,this,&mesage_widget::update_unread_message);
}

mesage_widget::~mesage_widget()
{
    delete ui;
}

void mesage_widget::setwidth(int w)
{

    this->setFixedWidth(w);

}

void mesage_widget::text_overflow(bool flag,QString &str)
{
    QLabel* l;
    if(flag)l=ui->name;
    else l=ui->message;
    QFont f=l->font();
    QFontMetrics metics(f);
    int max_width,w=0;
    max_width=l->width()-6-metics.horizontalAdvance("...");//获取可以显示的宽度
    for(int i=0;i<str.length();i++){
        w+=metics.horizontalAdvance(str[i]);
        if(w>max_width){
            l->setText(str.mid(0,i-1).append("..."));
            return ;
        }
    }
    qDebug()<<str<<endl;
    l->setText(str);
}

void mesage_widget::resizeEvent(QResizeEvent *event)
{
    text_overflow(true,this->name);
   text_overflow(false,this->message);
}
bool mesage_widget::eventFilter(QObject *watched, QEvent *event)
{
    if(watched==ui->head && event->type()==QEvent::Paint){
        this->painter();
    }
    return QWidget::eventFilter(watched,event);
}
void mesage_widget::painter()
{
    if(this->unread_message==0)return;
    qDebug()<<"painter函数调用了"<<endl;
    QPainter p(ui->head);
    QFontMetrics metrics(this->font());
    int w=2;
    if(this->unread_message>=10)w=0;
    p.setPen(Qt::red);
    p.setBrush(Qt::red);
    p.setRenderHints(QPainter::Antialiasing | QPainter::TextAntialiasing);
    //p.setRenderHint(,true);
    p.drawEllipse(QRect(38,0,9,9));
    p.setPen(Qt::white);
    if(this->unread_message<100){
      p.drawText(QRect(38+w,0,9,9),QString::number(this->unread_message));
    }else{
       p.drawText(QRect(38+w,0,9,9),"99");
    }
   // p.drawEllipse(QRect(0,0,45,45));

}
void mesage_widget::update_unread_message(int increase)
{
    if(increase>0)this->unread_message+=increase;
    else this->unread_message=0;
    update();
}

QString mesage_widget::getname()
{
    return name;
}

QString mesage_widget::getmessage()
{
    return message;
}
void mesage_widget::setname(QString &name)
{
    this->name=name;
}

void mesage_widget::setmessage(QString &message)
{
    this->message=message;
    this->text_overflow(false,this->message);
}

