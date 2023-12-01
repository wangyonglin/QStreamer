#include "MainWidget.h"

#include <QApplication>


#include <QObject>
#include<DexmuxThread.h>

#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWidget mainWidget;
    mainWidget.show();
    return a.exec();

}
