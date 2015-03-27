#include<iostream>
#include <QApplication>
#include "mygldrawer.h"


int main(int argc, char** argv)
{
  QApplication qapp(argc, argv);

#ifndef NDEBUG
  QSurfaceFormat format;
  format.setMajorVersion(4);
  format.setMinorVersion(3);
  format.setProfile(QSurfaceFormat::CoreProfile);
  format.setOption(QSurfaceFormat::DebugContext);

  QGLFormat glformat = QGLFormat::fromSurfaceFormat(format);
  MyGLDrawer widget(glformat);
#else
  MyGLDrawer widget();
#endif
  widget.show();
  int ret = qapp.exec();
  return ret;
}
