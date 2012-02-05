TEMPLATE = app
CONFIG +=  debug_and_release
SOURCES += main.cpp \
           src/mygldrawer.cpp
QT+= opengl

#for some reason the programmable pipeline API is defined as an extension
DEFINES+= GL_GLEXT_PROTOTYPES

INCLUDEPATH += inc \
               "/usr/include"

 CONFIG(debug, debug|release) {
    DESTDIR = debug
     TARGET = debug_game

 } else {
    DESTDIR = release
    TARGET = release_game
    DEFINES += NDEBUG
 }

HEADERS += \
    inc/mygldrawer.h
