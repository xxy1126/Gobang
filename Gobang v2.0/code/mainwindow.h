#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtWidgets>
#include <joinwindow.h>
#include <roomwindow.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButtonCreate_clicked();

    void on_pushButtonJoin_clicked();

    void on_pushButtonReturn_clicked();

private:
    Ui::MainWindow *ui;
    RoomWindow *roomWindow;
    JoinWindow *joinWindow;
    void paintEvent(QPaintEvent *);
};
#endif // MAINWINDOW_H
