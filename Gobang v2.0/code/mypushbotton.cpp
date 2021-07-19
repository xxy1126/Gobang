#include "mypushbotton.h"
#include <QDebug>
#include <QPropertyAnimation>


MyPushBotton::MyPushBotton(QString img)
{
    QPixmap pix;
    bool ret = pix.load(img);
    if(!ret)
    {
        qDebug() << "图片加载失败";
        return;
    }

    //设置图片固定大小
    this->setFixedSize(pix.width()*0.2,pix.height()*0.2);

    //设置不规则图片样式
    this->setStyleSheet("QPushButton{border:0px}");

    //设置图标
    this->setIcon(pix);

    //设置图标大小
    this->setIconSize(QSize(pix.width()*0.2,pix.height()*0.2));
}

void MyPushBotton::zoomup()
{
    //创建动态对象
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");

    //设置时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(),this->y(),this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y()+10,this->width(),this->height()));

    //设置弹跳效果
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();
}

void MyPushBotton::zoomdown()
{
    //创建动态对象
    QPropertyAnimation * animation = new QPropertyAnimation(this,"geometry");
    //设置时间间隔
    animation->setDuration(200);

    //起始位置
    animation->setStartValue(QRect(this->x(),this->y()+10,this->width(),this->height()));
    //结束位置
    animation->setEndValue(QRect(this->x(),this->y(),this->width(),this->height()));

    //设置弹跳效果
    animation->setEasingCurve(QEasingCurve::OutBounce);

    //执行动画
    animation->start();
}
