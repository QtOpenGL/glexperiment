#ifndef MYGLDRAWER_H
#define MYGLDRAWER_H

#include <QtGui>
#include<QtOpenGL>
#include<QGLWidget>
#include <assert.h>
#include "GL/gl.h"
#include "GL/glext.h"

#ifndef NDEBUG
    #define DEBUGMSG(...) fprintf(stdout, __VA_ARGS__)
#else
    #define DEBUGMSG(...)
#endif

class MyGLDrawer : public QGLWidget
 {
     Q_OBJECT        // must include this if you use Qt signals/slots

 public:
     MyGLDrawer(QWidget *parent=0)
         : QGLWidget(parent), m_pos_attr_loc(0)
     {
         show();
     }

     ~MyGLDrawer()
     {
        makeCurrent();
        glDeleteProgram(m_program);
        glDeleteShader(m_frag_shader);
        glDeleteShader(m_vert_shader);
        glDeleteBuffers(1, &m_vert_buffer);
        glDeleteBuffers(1, &m_indx_buffer);
     }

 protected:

     void initializeGL()
     {
         // Set up the rendering context, define display lists etc.:
         glEnable(GL_DEPTH_TEST);
         assert(setUpProgram());
         assert(setUpAttributes());

     }

     void resizeGL(int w, int h)
     {
        glViewport(0,0,w,h);
     }

     void paintGL()
     {
         // draw the scene:
         glClear(GL_COLOR_BUFFER_BIT);
         glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 0);
     }

     int setUpAttributes()
     {
        GLubyte indices[] = {0, 1, 2};

        GLfloat afVertices[] = {
                               -1.0f, -1.0f, 0.0f,
                                0.0f,  1.0f, 0.0f,
                                1.0f, -1.0f, 0.0f
                               };

        glGenBuffers(1, &m_vert_buffer);

        glBindBuffer(GL_ARRAY_BUFFER, m_vert_buffer);

        unsigned int uiSize = 3 * (sizeof(GLfloat) * 3);
        glBufferData(GL_ARRAY_BUFFER, uiSize, afVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(m_pos_attr_loc);
        glVertexAttribPointer(m_pos_attr_loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

        glGenBuffers(1, &m_indx_buffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indx_buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*sizeof(GLubyte), indices, GL_STATIC_DRAW);
        return 1;
     }

     int setUpProgram()
     {
         const char* pszFragShader = "\
                 void main (void)\
                 {\
                     gl_FragColor = vec4(1.0, 1.0, 0.66 ,1.0);\
                 }";

         const char* pszVertShader = "\
         attribute vec4	myVertex;\
         void main(void)\
         {\
             gl_Position = myVertex;\
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

         glBindAttribLocation(m_program, m_pos_attr_loc, "myVertex");

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

    GLuint m_program;
    GLuint m_frag_shader;
    GLuint m_vert_shader;
    GLuint m_vert_buffer;
    GLuint m_indx_buffer;
    GLint  m_pos_attr_loc;
 };
#endif // MYGLDRAWER_H
