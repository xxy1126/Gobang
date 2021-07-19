#include "login.h"
#include "ui_login.h"
#include "registerscene.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QSqlQuery>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QMessageBox>

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    //设置固定大小
    this->setFixedSize(30*5 + 16*40,30*2 +16*40);

    //设置图标
    this->setWindowIcon(QIcon(":/res/window.jpg"));

    //设置标题
    this->setWindowTitle("五子棋游戏");

    //登录按钮

    //注册按钮

    regscene = new RegisterScene;

    //监听完成信号
    connect(regscene,&RegisterScene::done,this,[=](){
        regscene->hide();
        this->setGeometry(regscene->geometry());
        this->show();
    });
}

Login::~Login()
{
    delete ui;
}

void Login::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing,true); // 抗锯齿

    //设置背景图片
    QPixmap pix;
    pix.load(":/res/bg.png");
    painter.drawPixmap(-300,-300,1500,1500,pix);
}

void Login::on_signin_btn_clicked()
{
    //连接数据库
    QSqlDatabase db;
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL");//数据库对象

    //连接数据库
    db.setHostName("127.0.0.1");    //设置为本地主机
    db.setUserName("root");         //设置用户名
    db.setPassword("mNMH4FngwKgi3Y2N");//设置密码
    db.setDatabaseName("accounts");   //选择数据库名称
    if(!db.open())
        QMessageBox::critical(this, QString::fromLocal8Bit("daa"), QString::fromLocal8Bit("fail!"), QString::fromLocal8Bit("no"));
//    else
//       QMessageBox::information(this, QString::fromLocal8Bit("info"), QString::fromLocal8Bit("succedd!"), QString::fromLocal8Bit("yes"));


    //获取登录信息
    QString user_name_i = ui->user_name->text();
    QString passwords_i = ui->passwords->text();

    if(user_name_i != "" && passwords_i != "")
    {
         QSqlQuery query1("select * from info");//查询表的内容
         while(query1.next())//一行一行遍历
         {
             QString user_name_o =query1.value(0).toString();
             QString passwords_o =query1.value(1).toString();

             if(user_name_o == user_name_i)
             {
                 flag_exist = true;
                 if(passwords_o == passwords_i)
                     flag_right = true;
             }
        }

        if(flag_exist && flag_right)
        {
            //  进入choosescene界面
            choscene  = new Choosescene;
            this->hide();
            choscene->setGeometry(this->geometry());
            choscene->show();
        }
        else if(!flag_exist)
            QMessageBox::information(this,"error","no such account");

        else if(!flag_right)
            QMessageBox::information(this,"error","incorrect password");

     }
    else
    {
        QMessageBox::information(this,"error","not input yet");
    }

}


void Login::on_registerbtn_clicked()
{
    this->hide();
    //位置一致
    regscene->setGeometry(this->geometry());
    regscene->show();
}


