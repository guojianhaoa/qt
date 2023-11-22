#ifndef CHAT_ITEM_H
#define CHAT_ITEM_H

#include <QWidget>
#include<QPainter>
#include<QDebug>
#include<QListWidgetItem>
namespace Ui {
class chat_item;
}

class chat_item : public QWidget
{
    Q_OBJECT

public:
    explicit chat_item(QString name,QString data="大家好我系渣渣辉，系兄弟就来砍我吧，爆率真的很高",int width=450);
    //
    ~chat_item();
    //void paintEvent(QPaintEvent *event) override;
    //bool eventFilter(QObject *);
    bool eventFilter(QObject *watched, QEvent *event) override;
    void text_line_break();
    void painter();
    void setWidth(int width,bool flag=true);
    int setid(int id);
    int getid(int id);
    QSize getSize();
    QString gettext();
    QFont font;
    QString data="";
private:
    Ui::chat_item *ui;
    int id;
};

#endif // CHAT_ITEM_H
