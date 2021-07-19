#ifndef LOGIN_H
#define LOGIN_H

#include "registerscene.h"
#include "choosescene.h"

#include <QWidget>

namespace Ui {
class Login;
}

class Login : public QWidget
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    bool flag_exist = false; //账号名是否存在
    bool flag_right = false; //若存在 密码是否正确

    Choosescene * choscene = NULL;

    RegisterScene * regscene = NULL;

    void connect_mysql();

    void paintEvent(QPaintEvent *event);


private slots:
    void on_signin_btn_clicked();

    void on_registerbtn_clicked();

private:
    Ui::Login *ui;
};

#endif // LOGIN_H
