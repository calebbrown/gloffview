/********************
 * FILE: render.h
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Header file for render.c. Defines the prototypes for the
 *     interface functions
 */

#ifndef _CB_RENDER_H
#define _CB_RENDER_H
#include <GL/gl.h>
#include "common.h"
#include "object.h"

/* interface function prototypes */
void init_render(object *, bool, render_type, int, int);
void render();
void resize(int,int);
void set_quaternion(float *);
void set_rotation(float,axis);

#endif /* !_CB_RENDER_H */
