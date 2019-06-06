#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
   // w.setGeometry(100, 0, 500, 800);
    w.initUi();
    w.show();

    return a.exec();
}
