/********************
 * FILE: object.h
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Header file for object.c. Defines the object structure and contains the
 *     prototypes for the interface functions
 */

#ifndef _CB_OBJECT_H
#define _CB_OBJECT_H

#include "face.h"
#include "vertex.h"

typedef struct {
  /* total number of vertices for this object */
  int n_vertices;

  /* the total number of faces for this object */
  int n_faces;

  /* how many we have loaded so far*/
  int filled_vertices;
  int filled_faces;

  /* the pointers to arrays for storing the faces and vertices */
  vertex *vertices;
  face *faces;

} object;

/* interface function prototypes */
bool init_object(object *,int,int);
void free_object(object *);
void add_vertex_p(object *,float,float,float,float,float,float);
void add_vertex(object *,vertex);
void add_face(object *,face);
void optimise(object *);
#endif /* !_CB_OBJECT_H */
