#ifndef REGISTERSCENE_H
#define REGISTERSCENE_H

#include <QMainWindow>

namespace Ui {
class RegisterScene;
}

class RegisterScene : public QMainWindow
{
    Q_OBJECT

public:
    explicit RegisterScene(QWidget *parent = nullptr);
    ~RegisterScene();

    bool flag_reg = true;

    void paintEvent(QPaintEvent *event);

private slots:
    void on_confi_Btn_clicked();

signals:
    void done();

private:
    Ui::RegisterScene *ui;
};

#endif // REGISTERSCENE_H
