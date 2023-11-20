#include "widget.h"

#include <QApplication>
#include <AudioRender.h>
#include <FFmpegManager.h>
#include <AudioContext.h>
#include <QObject>
#include<DexmuxThread.h>

#undef main
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();
    return a.exec();

}
