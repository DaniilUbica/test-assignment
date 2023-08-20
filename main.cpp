#include "server.h"
#include "client.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Server s;
    Client c;
    s.show();
    c.show();
    return a.exec();
}
