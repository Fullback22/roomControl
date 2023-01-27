#include "inputMenu.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    inputMenu w;
    w.show();
    return a.exec();
}
