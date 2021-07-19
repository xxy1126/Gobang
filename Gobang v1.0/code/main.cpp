#include "choosescene.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Choosescene w;
    w.show();
    return a.exec();
}
