#ifndef _CB_PLATFORM_H
#define _CB_PLATFORM_H

#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#else
#ifdef _WIN32 /* Unsupported! */
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#endif /* !_CB_PLATFORM_H */
