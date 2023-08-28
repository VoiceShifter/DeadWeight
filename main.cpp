#include "NoRecoil.hpp"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    NoRecoil w;
    w.show();
    return a.exec();
}
