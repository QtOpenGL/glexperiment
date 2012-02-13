#include "mygldrawer.h"
#include <iostream>

#ifndef NDEBUG
    #define DEBUGMSG(format, args...) fprintf(stdout, "DEBUG %s:%d: %s: "format, __FILE__, __LINE__,__FUNCTION__, ##args)
#else
    #define DEBUGMSG(format, args...)  void(0)
#endif


MyGLDrawer::MyGLDrawer(QWidget *parent): QGLWidget(parent), m_pos_attr_loc(0), m_tex_attr_loc(1), m_image(NULL)
{
  m_image = new QImage("resources/images/copper.jpg");
  m_image->convertToFormat(QImage::Format_ARGB32);
  DEBUGMSG("exists %d image: 0x%p %dx%d\n",QFile::exists(QString("resources/images/copper.jpg")), m_image, m_image->width(), m_image->height());
}

MyGLDrawer::~MyGLDrawer()
{
  makeCurrent();
  delete m_image;
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
   glEnable(GL_DEPTH_TEST);
   assert(setUpProgram());
   assert(setUpAttributes());
   assert(setUpUniforms());
}

void MyGLDrawer::resizeGL(int w, int h)
{
  glViewport(0,0,w,h);
}

void MyGLDrawer::paintGL()
{
   // draw the scene:
   glClear(GL_COLOR_BUFFER_BIT);
   glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 0);
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
  
  glActiveTexture(GL_TEXTURE0);
  glEnable(GL_TEXTURE_2D);
  glGenTextures(1, &m_tex_name);
  
  glBindTexture(GL_TEXTURE_2D, m_tex_name);
   
  glTexImage2D(GL_TEXTURE_2D, 0,  GL_RGB, m_image->width(), m_image->height(), 0, GL_RGB, GL_UNSIGNED_BYTE, m_image->bits());
  
  m_tex_uni_loc=glGetUniformLocation(m_program, "sTex");
  DEBUGMSG("GL Error: 0x%x\n", glGetError());
  glUniform1i(m_tex_uni_loc, 0);
  DEBUGMSG("GL Error: 0x%x\n", glGetError());
 
  return 1;
}

int MyGLDrawer::setUpProgram()
{
   const char* pszFragShader = "\
           varying vec2 vTexCoords;\
           uniform sampler2D sTex;\
           void main (void)\
           {\
               gl_FragColor = texture2D(sTex, vTexCoords);\
           }";

   const char* pszVertShader = "\
   attribute vec4	aVertex;\
   attribute vec2 aTexCoords;\
   varying vec2 vTexCoords;\
   void main(void)\
   {\
       gl_Position = aVertex;\
       vTexCoords = aTexCoords;\
   }";

   GLuint m_frag_shader, m_vert_shader;
   GLuint m_program;
   m_frag_shader = glCreateShader(GL_FRAGMENT_SHADER);

   glShaderSource(m_frag_shader, 1, (const char**)&pszFragShader, NULL);

   glCompileShader(m_frag_shader);

   GLint bShaderCompiled;
   glGetShaderiv(m_frag_shader, GL_COMPILE_STATUS, &bShaderCompiled);

   if (!bShaderCompiled)
   {
       int i32InfoLogLength, i32CharsWritten;
       glGetShaderiv(m_frag_shader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);

       char* pszInfoLog = new char[i32InfoLogLength];
       glGetShaderInfoLog(m_frag_shader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);

       DEBUGMSG("Failed to compile fragment shader: %s\n", pszInfoLog);
       delete [] pszInfoLog;
       return 0;
   }

   m_vert_shader = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(m_vert_shader, 1, (const char**)&pszVertShader, NULL);
   glCompileShader(m_vert_shader);
   glGetShaderiv(m_vert_shader, GL_COMPILE_STATUS, &bShaderCompiled);

   if (!bShaderCompiled)
   {
       int i32InfoLogLength, i32CharsWritten;
       glGetShaderiv(m_vert_shader, GL_INFO_LOG_LENGTH, &i32InfoLogLength);
       char* pszInfoLog = new char[i32InfoLogLength];
       glGetShaderInfoLog(m_vert_shader, i32InfoLogLength, &i32CharsWritten, pszInfoLog);
       DEBUGMSG("Failed to compile vertex shader: %s\n", pszInfoLog);
       delete [] pszInfoLog;
       return 0;
   }

   m_program = glCreateProgram();

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

  glClearColor(0.6f, 0.8f, 1.0f, 1.0f);//pvr blue!
  return 1;
}
