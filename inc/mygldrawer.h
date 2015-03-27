#ifndef MYGLDRAWER_H
#define MYGLDRAWER_H

#include <QtGui>
#include<QtOpenGL>
#include<QGLWidget>
#include<QImage>

class MyGLDrawer : public QGLWidget
{
  Q_OBJECT // must include this if you use Qt signals/slots

public:
  MyGLDrawer(const QGLFormat & format);
  MyGLDrawer(QWidget* parent=0);
  ~MyGLDrawer();

private slots:
  void handleLoggedMessage(QOpenGLDebugMessage);

private:
  void initializeGL();
  void resizeGL(int w, int h);

  void paintGL();

  bool event(QEvent * ev);

  int setUpAttributes();

  int setUpUniforms();

  int setUpProgram();

  int setUpTextures();

  int setupShaders();

  int setupGLDebug();

  int createShader(int, QString);

  void destroyArrayStuff();

  GLuint m_program;
  GLuint m_frag_shader;
  GLuint m_vert_shader;
  GLuint m_vert_buffer;
  GLuint m_indx_buffer;
  GLint  m_pos_attr_loc;
  GLint  m_tex_attr_loc;
  GLuint m_tex_name;
  GLint m_tex_uni_loc;
  QImage m_image;
  QOpenGLDebugLogger * m_logger;
  GLuint m_vao;
};
#endif // MYGLDRAWER_H
