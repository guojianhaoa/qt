#include "regster.h"
#include "ui_regster.h"

Regster::Regster(QTcpSocket *socket, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Regster)
{
    ui->setupUi(this);
    //注册按钮
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        //判断账号密码是否合法
        int n=ui->lineEdit->text().length()+ui->lineEdit_2->text().length();
        bool flag=false;
        if(n!=18){
            QMessageBox::critical(this,"错误提示","账号应为8位\n密码应为10位");
            flag=true;
        }
        else if(ui->lineEdit_3->text().length()>20){
            QMessageBox::critical(this,"错误提示","昵称过长");
            flag=true;
        }
        else{
            QString str=ui->lineEdit->text();
            for (int i=0;i<str.length();i++) {
                if((str[i]<'a'|| str[i]>'z') && (str[i]<'A' || str[i]>'Z') && (str[i]<'0' || str[i]>'9')){
                    flag=true;
                    QMessageBox::critical(this,"错误提示","账号只能为数字和字母");
                    break;
                }
            }
        }
        //向服务器发送注册信息
        if(!flag){
            QString str=QString("0%1%2%3").arg(ui->lineEdit->text()).arg(ui->lineEdit_2->text()).arg(ui->lineEdit_3->text());
            qDebug()<<"发送注册消息"<<endl;
            socket->write(str.toUtf8());
        }
    });
    //返回登录界面
    connect(ui->break_2,&QPushButton::clicked,[=](){
        emit breaklogo();
    });

    connect(this,&Regster::regster,[=](bool flag){
        if(flag){
            QMessageBox::information(this,"注册","账号注册成功");
        }
        else{
            QMessageBox::information(this,"注册","账号已注册");
        }
    });
}

Regster::~Regster()
{
    delete ui;
}


