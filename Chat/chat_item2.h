#ifndef CHAT_ITEM2_H
#define CHAT_ITEM2_H

#include <QWidget>
#include<QDebug>
#include<QPainter>
namespace Ui {
class chat_item2;
}

class chat_item2 : public QWidget
{
    Q_OBJECT

public:
    explicit chat_item2(QString name,QString data="大家好我系渣渣辉，系兄弟就来砍我吧，爆率真的很高",int width=450);
    ~chat_item2();
    bool eventFilter(QObject *watched, QEvent *event) override;
    void text_line_break();
    void painter();
    void setWidth(int width,bool flag=true);
    QSize getSize();
    QString gettext();
    QFont font;
    QString data="";
private:
    Ui::chat_item2 *ui;
};

#endif // CHAT_ITEM2_H
