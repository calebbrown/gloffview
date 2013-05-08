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
#include "common.h"
#include "platform.h"
#include "object.h"

typedef struct {
    /* Static globals we want hanging around */
    bool culling;
    render_type type;
    object *obj;
    int width;
    int height;

    /* Store the quaternion for changing the rotation of the object */
    float quat[4];
    GLfloat rot_history[4][4];
    float angle;
    axis  rot_axis;
    float zoom;

    /* Display List index, used when the display list option is chosen */
    int dl_index;
} renderer;

/* interface function prototypes */
renderer * init_render(object *, bool, render_type, int, int);
void render(renderer *);
void resize(renderer *,int,int);
void set_render_type(renderer *, render_type);
void set_quaternion(renderer *,float *);
void set_rotation(renderer *,float,axis);
void set_zoom(renderer *,float);
void set_culling(renderer * r, bool cull);
void reset_view(renderer *);

#endif /* !_CB_RENDER_H */
