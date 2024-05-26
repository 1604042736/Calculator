#include <QApplication>
#include <QWidget>

#include "Numerical.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    Numerical w;
    w.show();
    return app.exec();
}