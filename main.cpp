#include<iostream>
#include <QApplication>
#include "mygldrawer.h"


int main(int argc, char** argv)
{
    QApplication qapp(argc, argv);

    MyGLDrawer* widget = new MyGLDrawer();
    widget->show();
    int ret = qapp.exec();
    delete widget;
    return ret;
}
