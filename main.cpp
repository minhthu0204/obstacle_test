#include <QCoreApplication>
#include "ObstacleAvoidance.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ObstacleAvoidance obstacleAvoidance;
    obstacleAvoidance.run();
    return a.exec();
}
