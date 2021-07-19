#include "registerscene.h"
#include "ui_registerscene.h"

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QPainter>
#include <QSqlQuery>
#include <QMessageBox>
#include <QTimer>


RegisterScene::RegisterScene(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RegisterScene)
{
    ui->setupUi(this);

    //设置固定大小
    this->setFixedSize(30*5 + 16*40,30*2 +16*40);

    //设置图标
    this->setWindowIcon(QIcon(":/res/window.jpg"));

    //设置标题
    this->setWindowTitle("五子棋游戏");

    //完成按钮 储存账户
}

RegisterScene::~RegisterScene()
{
    delete ui;
}

void RegisterScene::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing,true); // 抗锯齿

    //设置背景图片
    QPixmap pix;
    pix.load(":/res/bg.png");
    painter.drawPixmap(-300,-300,1500,1500,pix);
}

void RegisterScene::on_confi_Btn_clicked()
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
    QString user_name_s = ui->user_name_s->text();
    QString passwords_s = ui->passwords_s->text();

    //账号不能重复 输入不能为空
    QSqlQuery query2("select * from info");
    while(query2.next())
    {
        QString user_name_o =query2.value(0).toString();
        if(user_name_o == user_name_s && user_name_s != "")
        {
            QMessageBox::information(this,"error","duplicate account id");
            flag_reg = false;
            break;
        }
    }


    if(user_name_s != "" && passwords_s != "" && flag_reg)
    {

        QString str = QString("insert into info(user_name,passwords) values('%1','%2')").arg(user_name_s).arg(passwords_s);

        QSqlQuery query3(str);//添加账号密码

        QMessageBox::information(this,"info","welcome!");

        QTimer::singleShot(300,this,[=](){
            //返回登录界面
            emit this->done();
        });

    }
    else if(user_name_s == "" || passwords_s == "")
    {
        QMessageBox::information(this,"error","not input yet");
    }


}

