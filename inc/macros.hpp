#include "GL/gl.h"
#include "GL/glext.h"
#include <stdio.h>
#include <QString>

#ifndef NDEBUG
  #define DEBUGMSG(format, args...) qDebug("DEBUG %s:%d: %s: "format, __FILE__, __LINE__,__FUNCTION__, ##args)

  #define VERIFYSTATUSFUNCDEC(TYPE)\
  int verify ## TYPE ## Status(GLuint handle)\


  #define VERIFYSTATUSFUNCDEF(TYPE, OP)\
  int verify ## TYPE ## Status(GLuint handle)\
  {\
    GLint bCompiled;\
    glGet ## TYPE ## iv(handle, GL_## OP  ##_STATUS, &bCompiled);\
    if (!bCompiled) \
    { \
      int i32InfoLogLength, i32CharsWritten; \
      glGet ## TYPE ## iv(handle, GL_INFO_LOG_LENGTH, &i32InfoLogLength);\
      char* pszInfoLog = new char[i32InfoLogLength];\
      glGet ## TYPE ## InfoLog(handle, i32InfoLogLength, &i32CharsWritten, pszInfoLog);\
      QString log = QString::fromLocal8Bit(const_cast<char*>(pszInfoLog), i32CharsWritten);\
      DEBUGMSG("Failed to compile " #TYPE ": %s\n", log.toLocal8Bit().constData()); \
      delete [] pszInfoLog;\
      return 0;\
    }\
    return 1;\
  }

  #define PRINTSHADERCODE(HANDLE)\
  GLsizei sz;\
  glGetShaderiv(HANDLE,GL_SHADER_SOURCE_LENGTH, &sz);\
  GLchar pText[sz];\
  glGetShaderSource(HANDLE, sz, &sz, pText);\
  DEBUGMSG("Shader (%d) code:\n---------\n%s\n---------\n",HANDLE, pText)

  #define VERIFYSTATUS(TYPE, HANDLE) verify ## TYPE ## Status(HANDLE)

#else
  #define DEBUGMSG(format, args...)  void(0)

  #define VERIFYSTATUSFUNCDEF(TYPE, OP)

  #define VERIFYSTATUSFUNCDEC(TYPE)

  #define VERIFYSTATUS(TYPE, HANDLE)  1

  #define PRINTSHADERCODE(HANDLE) void(0)
#endif

