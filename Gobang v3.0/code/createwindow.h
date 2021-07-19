#ifndef CREATEWINDOW_H
#define CREATEWINDOW_H

#include <QWidget>

namespace Ui {
class CreateWindow;
}

class CreateWindow : public QWidget
{
    Q_OBJECT

public:
    explicit CreateWindow(QWidget *parent = nullptr);
    ~CreateWindow();

private:
    Ui::CreateWindow *ui;
};

#endif // CREATEWINDOW_H
