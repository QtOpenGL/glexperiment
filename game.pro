TEMPLATE = app
SOURCES += src/main.cpp \
           src/mygldrawer.cpp \
           src/helper.cpp
QT+= opengl

#for some reason the programmable pipeline API is defined as an extension
DEFINES+= GL_GLEXT_PROTOTYPES

INCLUDEPATH += inc \
               "/usr/include"

 CONFIG(debug, debug|release) {

 } else {
    DEFINES += NDEBUG
 }

HEADERS += \
    inc/mygldrawer.h \
    inc/helper.hpp \
    inc/macros.hpp
    inc/helper.hpp

RESOURCES = res.qrc

DEPENDPATH = += ./resources
