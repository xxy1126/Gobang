#ifndef MYPUSHBOTTON_H
#define MYPUSHBOTTON_H

#include <QPushButton>

class MyPushBotton : public QPushButton
{
    Q_OBJECT
public:
    //explicit MyPushBotton(QWidget *parent = nullptr);

    //构造函数
    MyPushBotton(QString img);

    //弹跳特效
    void zoomup();
    void zoomdown();
signals:

};

#endif // MYPUSHBOTTON_H
