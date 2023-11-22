#include "mysqlite.h"

mySqlite::mySqlite(QString account)
{
    QString str=QString("./%1_chat_history.db").arg(account);
    qDebug()<<str<<endl;
    if(QSqlDatabase::contains(account)){
        this->datbase=QSqlDatabase::database();
    }else{
        this->datbase=QSqlDatabase::addDatabase("QSQLITE",account);
        qDebug()<<"创建了新连接"<<endl;
    }
    this->datbase.setDatabaseName(str);
    if(!this->datbase.open())qDebug()<<"数据库打开失败"<<endl;
    else{
        this->query=(QSqlQuery)this->datbase;
        qDebug()<<"数据库打开成功"<<endl;
    }
}

QStringList mySqlite::select_chat_history(QString account,int quantity,int position)
{
    QStringList chat_history;
    QString str=QString("SELECT * FROM %1 where id<=%2 and id>=%3").arg(account).arg(quantity+position).arg(position);
    qDebug()<<str<<endl;
    if(!query.exec(str)){
        qDebug()<<"查询表失败"<<endl;
         chat_history.append("false");
        return chat_history;
    }else{
        int length=0;
        //chat_history.append("true");
        while(query.next()){
           chat_history.append(query.value(0).toString());
           chat_history[length].append("-").append(query.value(1).toString()).append("-").append(query.value(2).toString()).append("-").append(query.value(3).toString());
           length++;
        }
    }
    qDebug()<<"select_chat_history"<<chat_history<<endl;
    return chat_history;
}

//QStringList mySqlite::select_table(QString table_name, QString where)
//{
//    QStringList data;
//    QString str;
//    if(where!=NULL)str=QString("select * from %1 where %2;").arg(table_name).arg(where);
//    else str=QString("select * from %1;").arg(table_name);
//    if(!query.exec(str)){
//        qDebug()<<"查询表失败"<<endl;
//         data.append("false");
//        return data;
//    }else{
//        int length=1;
//        int x=0;
//        QVariant vatiant;
//        data.append("true");
//        while(query.next()){
//           data.append("");
//           while(true){
//               vatiant=query.value(x++);
//               if(vatiant.isNull())break;
//               else data[length].append("-").append(vatiant.toString());
//           }
//           x=0;
//           length++;
//        }
//        return data;
//    }
//}

//QStringList mySqlite::select_table(QString table_name, QStringList& field, QString where)
//{
//    QStringList data;
//    QString str="select ";
////    if(where!=NULL)str=QString("select %1 from %2 where %3;").arg(field).arg(table_name).arg(where);
////    else str=QString("select %1 from %2;").arg(table_name).arg(table_name);
//    for(int i=0;i<field.length()-1;i++){
//        str.append(field[i]).append(",");
//    }
//    str.append(field[field.length()-1]).append(" from ").append(table_name);
//    if(where!=NULL)str.append(" where ").append(where);
//    qDebug()<<str<<endl;
//    if(!query.exec(str)){
//         qDebug()<<"查询表失败"<<endl;
//         data.append("false");
//        return data;
//    }else{
//        int length=1;
//        int x=0;
//        QVariant vatiant;
//        data.append("true");
//        while(query.next()){
//           data.append("");
//           while(true){
//               vatiant=query.value(x++);
//               if(vatiant.isNull())break;
//               else data[length].append("-").append(vatiant.toString());
//           }
//           x=0;
//           length++;
//        }
//        qDebug()<<"select_table:"<<data<<endl;
//        return data;
//    }
//}

QStringList mySqlite::select_table(QString table_name, QString field, QString where)
{
    QString str_select=QString("select %1 from %2").arg(field).arg(table_name);
    if(where!=NULL)str_select.append(" where ").append(where);
    QStringList data;
    qDebug()<<str_select<<endl;
    if(!query.exec(str_select)){
         qDebug()<<"查询表失败"<<endl;
         data.append("false");
        return data;
    }else{
        int length=1;
        int x=0;
        QVariant vatiant;
        data.append("true");
        while(query.next()){
           data.append("");
           while(true){
               vatiant=query.value(x++);
               if(vatiant.isNull())break;
               else data[length].append("-").append(vatiant.toString());
           }
           x=0;
           length++;
        }
        qDebug()<<"select_table:"<<data<<endl;
        return data;
    }
}

int mySqlite::insert_chat_history(QString account, char type, bool left_right, QString message)
{
    QString str_insert=QString("insert into %1 (type, left_right, message) values('%2',%3,'%4');").arg(account).arg(type).arg(left_right).arg(message);
    qDebug()<<str_insert<<endl;
    if(query.exec(str_insert)){
        return this->data_quantity(account);
    }else return -1;

}

bool mySqlite::insert(QString table_name, QString data)
{
    QString str_insert=QString("insert into %1 values(%2);").arg(table_name).arg(data);
    qDebug()<<str_insert<<endl;
    return query.exec(str_insert);
}
bool mySqlite::create_table(QString account)
{
    QString str_create=QString("create table %1(id INTEGER PRIMARY KEY,type char(1) not null,left_right bool not null,message text not null)").arg(account);
    qDebug()<<str_create<<endl;
    return query.exec(str_create);
}

bool mySqlite::create_table(QString table_name, QString field)
{
    QString str_create=QString("create table %1(%2)").arg(table_name).arg(field);
    qDebug()<<str_create<<endl;
    return query.exec(str_create);
}

int mySqlite::data_quantity(QString account)
{
    QString str_data=QString("select count(*) from %1;").arg(account);
    qDebug()<<str_data<<endl;
    query.exec(str_data);
    int x=0;
    while(query.next()){
        x=query.value(0).toInt();
    }
    qDebug()<<"create_table"<<x<<endl;
    return x;
}

bool mySqlite::update(QString table_name, QString update_field, QString where)
{
    QString str_update=QString("update %1 set %2").arg(table_name).arg(update_field);
    if(where!=NULL)str_update.append(" where ").append(where);
     qDebug()<<str_update<<endl;

     return this->query.exec(str_update);
}


