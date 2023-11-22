#ifndef MESAGE_WIDGET_H
#define MESAGE_WIDGET_H

#include <QWidget>
#include<QFont>
#include<QLabel>
#include<QDebug>
#include<QPainter>
namespace Ui {
class mesage_widget;
}

class mesage_widget : public QWidget
{
    Q_OBJECT

public:
    explicit mesage_widget(QString &name ,QString &message,QStringList* chat_histor=NULL);
    ~mesage_widget();
    void setwidth(int w);//
    void text_overflow(bool flag,QString &str);//处理文本溢出,当文本溢出时多余的以...代替,flag表示操作对象true为name,false表示message
    void resizeEvent(QResizeEvent *event) override;
    bool eventFilter(QObject *watched,QEvent *event) override;
    void painter();
    void update_unread_message(int increase);
    void setname(QString &name);
    void setmessage(QString &message);
    QString getname();
    QString getmessage();
private:

    Ui::mesage_widget *ui;
    QString name;
    QString message;
    int unread_message=0;
signals:
    //void upate_unread_message(int increase);
public slots:
    //void update_unread_message(int increase);
};

#endif // MESAGE_WIDGET_H
