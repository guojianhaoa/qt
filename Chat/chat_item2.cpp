#include "chat_item2.h"
#include "ui_chat_item2.h"

chat_item2::chat_item2(QString name,QString data,int width) :

    ui(new Ui::chat_item2)
{
    ui->setupUi(this);
    qDebug()<<"创建"<<endl;
    this->data=data;//聊天信息
    //ui->label_2->setText(name);
    //this->setParent(parent);
    ui->widget->layout()->setAlignment(Qt::AlignLeft);
    this->setFixedWidth(width);
    ui->widget_2->installEventFilter(this);//开启事件过滤器
    font=ui->textEdit->font();
    font.setFamily("微软雅黑");
    font.setPixelSize(15);
    ui->textEdit->setFont(font);
    ui->textEdit->setStyleSheet("border-style:solid;background-color: rgba(7, 220, 245,255);");//
    QPixmap picmap(":/head.jpg");
    QPixmap p=picmap.scaled(ui->label->size(),Qt::KeepAspectRatio);
    ui->label->setPixmap(p);
    text_line_break();
}

chat_item2::~chat_item2()
{
    delete ui;
}

bool chat_item2::eventFilter(QObject *watched, QEvent *event)
{
    //qDebug()<<"触发了事件"<<event->type()<<endl;
    if(watched==ui->widget_2 && event->type()==QEvent::Paint){
        painter();
    }
//    if(watched==ui->widget_2 && event->type()==QEvent::Wheel){

//        qDebug()<<"过滤文本框滚动事件"<<endl;
//    }
    return QWidget::eventFilter(watched,event);
}

void chat_item2::text_line_break()
{
    QFont font(ui->textEdit->font());
    QFontMetrics metrics(font);
    int text_break=0;//换行
    int textline;//文本框宽度
    int text_width=0;//字符宽度
    int highet=0;//每行字符高度的总和
    int max_width=0;//各行里最大的字符宽度
    bool flag=false;//输入的文本是否有带有换行符
    double d=this->width()*0.7;
   // qDebug()<<font<<endl;
    ui->widget_2->setMaximumWidth(d-45-9);
    ui->widget_2->setMinimumWidth(d-45-9);
    ui->textEdit->setText("");
    textline=d-45-9-9-19;
    //qDebug()<<"旧width:"<<ui->widget_2->width()<<endl;
    for(int i=0;i<data.length();i++){
        if(data[i]=='\n'){
            if(text_width>max_width)max_width=text_width;
            text_width=0;
            flag=true;
            highet+=metrics.lineSpacing();
            continue;
         }
      // qDebug()<<data[i];
       text_width+= metrics.horizontalAdvance(data[i]);
       if(text_width>textline){
           ui->textEdit->insertPlainText(data.mid(text_break,i-text_break));
           ui->textEdit->insertPlainText("\n");
           if(text_width-metrics.horizontalAdvance(data[i])>max_width)max_width=text_width-metrics.horizontalAdvance(data[i]);
           text_width=metrics.horizontalAdvance(data[i]);
           highet+=metrics.lineSpacing();
           text_break=i;

       }

       //ui->textEdit->insertPlainText(data[i]);
    }
    if(text_width>max_width)max_width=text_width;
    ui->textEdit->insertPlainText(data.mid(text_break,data.length()-text_break));
    highet+=metrics.lineSpacing();
    //qDebug()<<"字符宽度:"<<text_width<<endl;
    this->setFixedHeight(highet+9+9+20+8);
    if(highet==metrics.lineSpacing()){
        int width=text_width+19+9+10;
        ui->widget_2->setFixedWidth(width);

    }
    else{
        int width=max_width+19+9+10;
        ui->widget_2->setFixedWidth(width);
       // qDebug()<<"widget"<<ui->widget_2->height()<<endl;
    }
}

void chat_item2::painter()
{
    QPainter painter(ui->widget_2);
       painter.setRenderHint(QPainter::Antialiasing,true);
       painter.setPen(QColor(7, 220, 245,255));
       painter.setBrush(QColor(7, 220, 245,255));
      // painter.begin(ui->widget_2);
       QRect rect(12,5,ui->widget_2->width()-13,ui->widget_2->height()-9);//int x,int y,int width,int heigth
       painter.drawRect(rect);
       painter.drawRoundedRect(rect,8,Qt::RelativeSize);
       QPointF abc[3]={QPointF(12,15),
                       QPointF(6,21),
                       QPointF(12,27)};
       painter.drawPolygon(abc,3);

}

void chat_item2::setWidth(int width, bool flag)
{
    if(flag)this->setFixedWidth(this->width()+width);
    else setFixedWidth(width);
}

QSize chat_item2::getSize()
{
     return QSize(this->width(),this->height());
}

QString chat_item2::gettext()
{
    return ui->textEdit->toPlainText();
}
