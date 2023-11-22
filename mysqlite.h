#ifndef MYSQLITE_H
#define MYSQLITE_H

#include <QObject>
#include<QDebug>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QVariant>
#include<QSqlError>
#include"mesage_widget.h"
//用于管理sqlite，将聊天记录存储在本地
class mySqlite : public QObject
{
    Q_OBJECT
public:
    explicit mySqlite(QString account);
    QStringList select_chat_history(QString account,int quantity=100,int position=0);//返回从position开始的quantity条聊天记录
    //QStringList select_table(QString table_name,QString where=NULL);//用于查询表；
    //QStringList select_table(QString table_name,QStringList& field,QString where=NULL);//参数为 表名 查询的字段名 查询条件
    QStringList select_table(QString table_name,QString field,QString where=NULL);
    //select_table 返回的strinlist里的每一条string格式为 -字段-字段-字......
    int insert_chat_history(QString account,char type,bool left_right,QString message);//插入聊天记录
    bool insert(QString table_name,QString data);//插入数据 表名 数据
    bool create_table(QString account);//建聊天记录信息表 表名
    bool create_table(QString table_name,QString field);//建表 表名 字段
    int data_quantity(QString account);//返回聊天记录条数
    bool update(QString table_name,QString update_field,QString where=NULL);//表名  更新的字段及值  条件
private:
    QSqlDatabase datbase;
    QSqlQuery query;
signals:

};

#endif // MYSQLITE_H
