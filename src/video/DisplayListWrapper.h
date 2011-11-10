#ifndef PGL_WRAPPER
#define PGL_WRAPPER

#include "GLES/gl.h"
#include <vector>
#include <map>

typedef std::vector< std::vector<GLfloat> > PglList;

GLuint pglGenLists(int number);
void pglNewList(GLuint list);
void pglWriteList(std::vector<GLfloat> *verts);
void pglEndList();
void pglCallList(GLuint list);
void pglDeleteLists(GLuint list, int number);

#endif
