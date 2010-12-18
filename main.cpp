#include <QtGui/QApplication>
#include "diff.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Diff w;
    w.show();
    return a.exec();
}
