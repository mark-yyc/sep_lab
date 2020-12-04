#include "router.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Router router;
    router.show();
    return a.exec();
}
