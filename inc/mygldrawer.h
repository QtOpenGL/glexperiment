#ifndef MYGLDRAWER_H
#define MYGLDRAWER_H

#include <QtGui>
#include<QtOpenGL>
#include<QGLWidget>
#include<QImage>
#include<QFile>
#include <assert.h>
#include "GL/gl.h"
#include "GL/glext.h"

class MyGLDrawer : public QGLWidget
 {
     Q_OBJECT        // must include this if you use Qt signals/slots

 public:
     MyGLDrawer(QWidget *parent=0);

     ~MyGLDrawer();
 protected:

     void initializeGL();

     void resizeGL(int w, int h);

     void paintGL();

     bool event(QEvent * ev);

     int setUpAttributes();
     
     int setUpUniforms();

     int setUpProgram();

    GLuint m_program;
    GLuint m_frag_shader;
    GLuint m_vert_shader;
    GLuint m_vert_buffer;
    GLuint m_indx_buffer;
    GLint  m_pos_attr_loc;
    GLint  m_tex_attr_loc;
    uint m_tex_name;
    int m_tex_uni_loc;
    QImage* m_image;
 };
#endif // MYGLDRAWER_H
