#include "mygldrawer.h"
#include <iostream>

#ifndef NDEBUG
    #define DEBUGMSG(format, args...) printf("DEBUG %s:%d: %s: "format, __FILE__, __LINE__,__FUNCTION__, ##args)
#else
    #define DEBUGMSG(format, args...)  void(0)
#endif


MyGLDrawer::MyGLDrawer(QWidget *parent): QGLWidget(parent), m_pos_attr_loc(0), m_tex_attr_loc(1)
{
  m_image = QGLWidget::convertToGLFormat(QImage(":/copper.jpg"));
  DEBUGMSG("exists %d image  %dx%d\n",QFile::exists(QString(":/copper.jpg")), m_image.width(), m_image.height());
}

MyGLDrawer::~MyGLDrawer()
{
  makeCurrent();
  glDeleteProgram(m_program);
  glDeleteShader(m_frag_shader);
  glDeleteShader(m_vert_shader);
  glDeleteBuffers(1, &m_vert_buffer);
  glDeleteBuffers(1, &m_indx_buffer);
  glDeleteTextures(1,&m_tex_name);
}

void MyGLDrawer::initializeGL()
{
   // Set up the rendering context, define display lists etc.:
   glClearColor(0.6f, 0.8f, 1.0f, 1.0f);//pvr blue!
   glEnable(GL_DEPTH_TEST);

   bool shaderok = setupShaders();
   assert(shaderok);

   bool progok = setUpProgram();
   assert(progok);

   bool attribok = setUpAttributes();
   assert(attribok);

   bool texok = setUpTextures();
   assert(texok);

   bool uniok = setUpUniforms();
   assert(uniok);
}

void MyGLDrawer::resizeGL(int w, int h)
{
  glViewport(0,0,w,h);
}

void MyGLDrawer::paintGL()
{
    // draw the scene:
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 0);
}

bool MyGLDrawer::event(QEvent * ev)
{
    if(ev->type() == QEvent::KeyPress)
    {
        QKeyEvent *qe((QKeyEvent*)ev);
        switch(qe->key())
        {
            case Qt::Key_Escape:
            case Qt::Key_Q:
            QWidget::close();
            break;
        }
    }
    return QGLWidget::event(ev);
}

int MyGLDrawer::setUpAttributes()
{
  GLubyte indices[] = {0, 1, 2};

  GLfloat attrData[] = {
                         -1.0f, -1.0f, 0.0f,
                         0.0f, 0.0f,
                          0.0f,  1.0f, 0.0f,
                         0.5f, 1.0f,
                          1.0f, -1.0f, 0.0f,
                         1.0f, 1.0f
                         };

  glGenBuffers(1, &m_vert_buffer);

  glBindBuffer(GL_ARRAY_BUFFER, m_vert_buffer);

  unsigned int uiSize = 3 * (sizeof(GLfloat) * 5);
  glBufferData(GL_ARRAY_BUFFER, uiSize, attrData, GL_STATIC_DRAW);
  glEnableVertexAttribArray(m_pos_attr_loc);
  glEnableVertexAttribArray(m_tex_attr_loc);
  glVertexAttribPointer(m_pos_attr_loc, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, 0);
  glVertexAttribPointer(m_tex_attr_loc, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, reinterpret_cast<void*>(sizeof(GLfloat) * 3));

  glGenBuffers(1, &m_indx_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indx_buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLubyte), indices, GL_STATIC_DRAW);
  
  return 1;
}

int MyGLDrawer::setUpUniforms()
{
    m_tex_uni_loc=glGetUniformLocation(m_program, "sTex");
    assert(m_tex_uni_loc != -1);
    glUniform1i(m_tex_uni_loc, 0);
  return 1;
}

int MyGLDrawer::setUpTextures()
{
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1, &m_tex_name);
    glBindTexture(GL_TEXTURE_2D, m_tex_name);

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);


    glTexImage2D(GL_TEXTURE_2D,
                 0,
                 GL_RGBA,
                 m_image.width(),
                 m_image.height(),
                 0,
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 m_image.bits());

    return 1;
}



int MyGLDrawer::setupShaders()
{
  m_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);
  QString fragpath(":/default.frag");
  m_vert_shader = glCreateShader(GL_VERTEX_SHADER);
  QString vertpath(":/default.vert");
  return createShader(m_frag_shader, fragpath) && createShader(m_vert_shader, vertpath);
}

int MyGLDrawer::createShader(int shaderid, QString shaderpath)
{
  QFile s(shaderpath);
  s.open(QFile::ReadOnly | QFile::Text);
  QTextStream in(&s);
  QString str = in.readAll();
  const char * pchar = str.toLocal8Bit().constData();
  glShaderSource(shaderid, 1,(const char**)&pchar, NULL);

  glCompileShader(shaderid);

  GLint bShaderCompiled;
  glGetShaderiv(shaderid, GL_COMPILE_STATUS, &bShaderCompiled);

  s.close();
  if (!bShaderCompiled)
  {
     int i32InfoLogLength, i32CharsWritten;
     glGetShaderiv(shaderid, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

     char* pszInfoLog = new char[i32InfoLogLength];
     glGetShaderInfoLog(shaderid, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

     DEBUGMSG("Failed to compile shader: %s\n", pszInfoLog);
     delete [] pszInfoLog;
     return 0;
  }
  return 1;
}

int MyGLDrawer::setUpProgram()
{
   m_program = glCreateProgram();
   DEBUGMSG( "m_program : %d \n", m_program);
   glAttachShader(m_program, m_frag_shader);
   glAttachShader(m_program, m_vert_shader);

   glBindAttribLocation(m_program, m_pos_attr_loc, "aVertex");
   glBindAttribLocation(m_program, m_tex_attr_loc, "aTexCoords");

   glLinkProgram(m_program);

   GLint bLinked;
   glGetProgramiv(m_program, GL_LINK_STATUS, &bLinked);

   if (!bLinked)
   {
       int ui32InfoLogLength, ui32CharsWritten;
       glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &ui32InfoLogLength);
       char* pszInfoLog = new char[ui32InfoLogLength];
       glGetProgramInfoLog(m_program, ui32InfoLogLength, &ui32CharsWritten, pszInfoLog);
       DEBUGMSG("Failed to link program: %s\n", pszInfoLog);
       delete [] pszInfoLog;
       return 0;
   }

  glUseProgram(m_program);
  return 1;
}
