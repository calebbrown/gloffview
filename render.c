/********************
 * FILE: render.c
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 28-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Functions for handling rendering. From initialisation to drawing the
 *     desired object to applying quaternions to it.
 */

#include <stdio.h>
#include <stdlib.h> /*for malloc*/

#include "common.h"
#include "platform.h"
#include "render.h"
#include "object.h"
#include "face.h"
#include "vertex.h"
#include "trackball.h"


/* Function prototypes for non interface functions */
void init_display_list(renderer *);
void init_vertex_array(renderer *);
void render_normal(renderer *);
void render_vertex_array(renderer *);


/* init_render():
   description: initialises the render ready for rendering
   inputs: pointer to an allocated object to render
           true/false to do back face culling
           the type of rendering, either normal, display lists or vertex arrays
 */
renderer * init_render(object *o, bool back_cull, render_type t, int w, int h){
  renderer *r;

  r = (renderer *) malloc(sizeof(renderer));

  /* Store the given parameters */
  r->obj = o;
  r->culling = back_cull;
  r->type = t;

  reset_view(r);

  /* Call render type specific initialisation code */
  if(r->type==display_list)
    init_display_list(r);
  else if(r->type==vertex_array)
    init_vertex_array(r);

  return r;
}


/* render():
   description: draws the given object :)
 */
void render(renderer * r){
  float lightpos[] = {5.0f,5.0f,5.0f,0.0f};
  float lightcolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float objectmat[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float zero[] = {0.0f, 0.0f, 0.0f, 1.0f};
  GLfloat matrix[4][4];

  if(r == NULL) return;

  /* FIX ME Dirty hack to stop resize bug */
  resize(r,r->width,r->height);

  /* Enable it depending on the desired setting */
  if(r->culling == true)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* Setup the viewer transform */
  gluLookAt(0.0,0.0,5.0 * r->zoom,
            0.0,0.0,0.0,
            0.0,1.0,0.0);

  /* Clear everything to 0,0,0,0 */
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  /* Setup the lighting */
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, zero);
  glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, lightcolor);
  glEnable(GL_LIGHT0);

  /* Setup the colours on the object */
  glMaterialfv(GL_FRONT, GL_DIFFUSE, objectmat);
  glMaterialfv(GL_FRONT, GL_SPECULAR, zero);
  glMaterialf(GL_FRONT, GL_SHININESS, 0);

  /* Draw the object */
  glPushMatrix();

  /* insert transforms here! */
  /*if((r->quat[0] == 0) &&
     (r->quat[1] == 0) &&
     (r->quat[2] == 0) &&
     (r->quat[3] == 0)){*/

      // rotate the angles

  build_rotmatrix(matrix,r->quat);
  glMultMatrixf(&matrix[0][0]);

  glMultMatrixf(&(r->rot_history[0][0]));

  if(r->rot_axis == x)
    glRotatef(r->angle,1,0,0);
  else if(r->rot_axis == y)
    glRotatef(r->angle,0,1,0);
  else if(r->rot_axis == z)
    glRotatef(r->angle,0,0,1);


  /* Render the object using desired method */
  if(r->type == normal)
    render_normal(r);
  else if(r->type == display_list)
    glCallList(r->dl_index);
  else
    render_vertex_array(r);

  glPopMatrix();
  glFlush();
  glutSwapBuffers();
}

void reset_view(renderer *r){
  int i,j;
  r->zoom = 1.0;

  /* Zero the quaternion so it doesn't affect anything */
  r->quat[0] = r->quat[1] = r->quat[2] = r->quat[3] = 0;

  r->angle = 0;

  /* Load identity */
  for(i =0; i < 4; i++)
    for(j =0; j < 4; j++)
      r->rot_history[i][j] = (i == j) ? 1.0 : 0.0;
}


/* resize():
   description: handles the resizing (and init) of the window or viewport
   inputs: width and height
 */
void resize(renderer * r, int w,int h){
  if(r == NULL) return;

  r->width = w;
  r->height = h;

  glViewport(0,0,r->width,r->height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, 1.0, 1.0, 40.0);

  /* Enable stuff */
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_LIGHTING);

  /* Use Gouraud shading */
  glShadeModel(GL_SMOOTH);

  /* Set the face to cull */
  glCullFace(GL_BACK);

  /* set the debth test func */
  glDepthFunc(GL_LESS);

  /* Kinda redudant, but what the hey! */
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/* set_quaternion:
   description: sets the quaternion for transforming the object, this probably
                should be stored with the object rather than in the render code
   inputs: the quaternion (4dimensional array of floats)
 */
void set_quaternion(renderer * r,float *q){
  if(r == NULL) return;
  r->quat[0] = q[0];
  r->quat[1] = q[1];
  r->quat[2] = q[2];
  r->quat[3] = q[3];
}

void set_render_type(renderer * r, render_type t) {
    r->type = t;
}

/* set_rotation():
   description: rotates by angle around the specified axis
   inputs: the angle, degrees to change and the axis
 */
void set_rotation(renderer * r,float da,axis rot){
  if(r == NULL) return;

  if(r->rot_axis != rot) {
    /* save the history, use the texture matrix stack */
    glMatrixMode(GL_TEXTURE);
    glLoadMatrixf(&(r->rot_history[0][0]));

    if(r->rot_axis == x)
      glRotatef(r->angle,1,0,0);
    else if(r->rot_axis == y)
      glRotatef(r->angle,0,1,0);
    else if(r->rot_axis == z)
      glRotatef(r->angle,0,0,1);

    glGetFloatv(GL_TEXTURE_MATRIX,&(r->rot_history[0][0]));

    /* change the rotation */
    r->rot_axis = rot;
    r->angle = 0;
  }

  r->angle += da;
  r->angle = r->angle >= DEGREESPERREV ? r->angle - DEGREESPERREV : r->angle;
}

void set_zoom(renderer * r,float dz) {
  if(r == NULL) return;
  r->zoom += dz;
}

void set_culling(renderer * r, bool cull) {
  r->culling = cull;
}


/* render_normal():
   description: draws the object using the normal rendering technique.
                ie: without any fancy rendering
 */
void render_normal(renderer * r){
  int i,j;
  face f;
  vertex v;

  /* draw our model*/
  for(i=0;i<(r->obj)->n_faces;i++){
    f = (r->obj)->faces[i];

    glBegin(f.draw_mode);
    glColor3f(f.colour[0],f.colour[1],f.colour[2]);

    for(j=0;j<f.n_vertices;j++){
      v = (r->obj)->vertices[f.vertex_indices[j]];
      glNormal3f(v.normX,v.normY,v.normZ);
      glVertex3f(v.x,v.y,v.z);
    }

    glEnd();
  }
}


/* render_vertex_array():
   description: draws the object using vertex arrays, which have been setup
                earlier
 */
void render_vertex_array(renderer * r){
  int i;
  face f;

  for(i =0 ; i < (r->obj)->n_faces ; i++) {
    f = (r->obj)->faces[i];

    glColor3f(f.colour[0],f.colour[1],f.colour[2]);

    /* The machine that does the work. Draw I Say! */
    glDrawElements(f.draw_mode,f.n_vertices,GL_UNSIGNED_INT,f.vertex_indices);
  }
}


/* init_vertex_array():
   description: prepares the render for drawing using vertex arrays
 */
void init_vertex_array(renderer * r){
  float *vertices;

  /* recast our vertex struct as an array of floats
     fortunetly the vertex struct is setup in such a way that this works */
  vertices = (float *) (r->obj)->vertices;

#ifdef INDEX_OPTIMISATION
  /* rejig the object so we can save time on calls! */
  optimise(obj);
#endif /* INDEX_OPTIMISATION */

  /* Enable the vertex and normal array states so GL knows what to do */
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  /* Why mess around with glVertexPointer and glNormalPoint when GL gives
     you this function to work with? */
  /* Setup the array ready for use */
  glInterleavedArrays(GL_N3F_V3F,0,vertices);

}


/* init_display_list():
   description: prepares a display list for drawing the object
 */
void init_display_list(renderer * r){
  r->dl_index = glGenLists(1);

  glNewList(r->dl_index,GL_COMPILE);

  render_normal(r);

  glEndList();
}
