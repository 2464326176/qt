#include "widget.h"

#include <QApplication>
#include "clientwidget.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();

    clientwidget w2;
    w2.show();
    return a.exec();
}
