#ifndef CHOOSESCENE_H
#define CHOOSESCENE_H

#include "mainscene.h"
#include "mainwindow.h"
#include <QWidget>
#include <QPainter>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Choosescene : public QWidget
{
    Q_OBJECT

public:
    Choosescene(QWidget *parent = nullptr);
    ~Choosescene();

private:
    Ui::Widget *ui;

    void paintEvent(QPaintEvent *event);

    Mainscene * scene = NULL;
    MainWindow * mainWindow;
};
#endif // CHOOSESCENE_H
