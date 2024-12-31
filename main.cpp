#include <QCoreApplication>
#include "DisplayManager.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    DisplayManager displayManager;
    displayManager.run();
    return a.exec();
}
