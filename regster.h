#ifndef REGSTER_H
#define REGSTER_H

#include <QMainWindow>
#include<QMessageBox>
#include<QTcpSocket>
#include<QElapsedTimer>
namespace Ui {
class Regster;
}

class Regster : public QMainWindow
{
    Q_OBJECT

public:
    explicit Regster(QTcpSocket* socket, QWidget *parent = nullptr);
    ~Regster();
    QString accont,password,name;

signals:
    void breaklogo();//返回登录界面信号
    void regster(bool flag);
private:
    Ui::Regster *ui;
};

#endif // REGSTER_H
