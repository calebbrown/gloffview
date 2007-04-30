/********************
 * FILE: render.c
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 28-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Functions for handling rendering. From initialisation to drawing the
 *     desired object to applying quaternions to it.
 */

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <stdio.h>

#include "common.h"
#include "render.h"
#include "object.h"
#include "face.h"
#include "vertex.h"
#include "trackball.h"


/* Function prototypes for non interface functions */
void init_display_list();
void init_vertex_array();
void render_normal();
void render_vertex_array();

/* Static globals we want hanging around */
static bool culling;
static render_type type;
static object *obj;
static int width;
static int height;

/* Store the quaternion for changing the rotation of the object */
static float quat[4];
static float angle;
static axis  rot_axis;

/* Display List index, used when the display list option is chosen */
static int dl_index;


/* init_render():
   description: initialises the render ready for rendering
   inputs: pointer to an allocated object to render
           true/false to do back face culling
           the type of rendering, either normal, display lists or vertex arrays
 */
void init_render(object *o, bool back_cull, render_type t, int w, int h){
  /* Store the given parameters */
  obj = o;
  culling = back_cull;
  type = t;

  /* Zero the quaternion so it doesn't affect anything */
  quat[0] = quat[1] = quat[2] = quat[3] = 0;

  /* Call render type specific initialisation code */
  if(type==display_list)
    init_display_list();
  else if(type==vertex_array)
    init_vertex_array();

}


/* render():
   description: draws the given object :)
 */
void render(){
  float lightpos[] = {5.0f,5.0f,5.0f,0.0f};
  float lightcolor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float objectmat[] = {1.0f, 1.0f, 1.0f, 1.0f};
  float zero[] = {0.0f, 0.0f, 0.0f, 1.0f};
  GLfloat matrix[4][4];

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /* Setup the viewer transform */
  gluLookAt(0.0,0.0,5.0,
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
  if((quat[0] == 0) &&
     (quat[1] == 0) &&
     (quat[2] == 0) &&
     (quat[3] == 0)){

    if(rot_axis == x) {
      glRotatef(angle,1,0,0);
    } else if (rot_axis == y) {
      glRotatef(angle,0,1,0);
    } else if (rot_axis == z) {
      glRotatef(angle,0,0,1);
    }

  } else {
    build_rotmatrix(matrix,quat);
    glMultMatrixf(&matrix[0][0]);
  }


  /* Render the object using desired method */
  if(type == normal)
    render_normal();
  else if(type == display_list)
    glCallList(dl_index);
  else
    render_vertex_array();

  glFlush();
  glutSwapBuffers();
}


/* resize():
   description: handles the resizing (and init) of the window or viewport
   inputs: width and height
 */
void resize(int w,int h){
  width = w;
  height = h;

  glViewport(0,0,width,height);
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

  /* Enable it depending on the desired setting */
  if(culling == true)
    glEnable(GL_CULL_FACE);
  else
    glDisable(GL_CULL_FACE);

  /* Kinda redudant, but what the hey! */
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


/* set_quaternion:
   description: sets the quaternion for transforming the object, this probably
                should be stored with the object rather than in the render code
   inputs: the quaternion (4dimensional array of floats)
 */
void set_quaternion(float *q){
  quat[0] = q[0];
  quat[1] = q[1];
  quat[2] = q[2];
  quat[3] = q[3];
}


/* set_rotation():
   description: rotates by angle around the specified axis
   inputs: the angle and the axis
 */
void set_rotation(float a,axis r){
  rot_axis = r;
  angle = a;
  quat[0] = 0;
  quat[1] = 0;
  quat[2] = 0;
  quat[3] = 0;
}

/* render_normal():
   description: draws the object using the normal rendering technique.
                ie: without any fancy rendering
 */
void render_normal(){
  int i,j;
  face f;
  vertex v;

  /* draw our model*/
  for(i=0;i<obj->n_faces;i++){
    f = obj->faces[i];

    glBegin(f.draw_mode);
    glColor3f(f.colour[0],f.colour[1],f.colour[2]);

    for(j=0;j<f.n_vertices;j++){
      v = obj->vertices[f.vertex_indices[j]];
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
void render_vertex_array(){
  int i;
  face f;

  for(i =0 ; i < obj->n_faces ; i++) {
    f = obj->faces[i];

    glColor3f(f.colour[0],f.colour[1],f.colour[2]);

    /* The machine that does the work. Draw I Say! */
    glDrawElements(f.draw_mode,f.n_vertices,GL_UNSIGNED_INT,f.vertex_indices);
  }
}


/* init_vertex_array():
   description: prepares the render for drawing using vertex arrays
 */
void init_vertex_array(){
  float *vertices;

  /* recast our vertex struct as an array of floats
     fortunetly the vertex struct is setup in such a way that this works */
  vertices = (float *) obj->vertices;

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
void init_display_list(){
  dl_index = glGenLists(1);

  glNewList(dl_index,GL_COMPILE);

  render_normal();

  glEndList();
}
