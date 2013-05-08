/********************
 * FILE: face.h
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Header file for face.c. Defines the face structure and contains the
 *     prototypes for the interface functions
 */

#ifndef _CB_FACE_H
#define _CB_FACE_H
#include "platform.h"
#include "common.h"

typedef struct face_t {
  /* array of indices into a vertex array */
  int *vertex_indices;

  /* the number of vertices for this face */
  int n_vertices;

  /* the number of vertices we've added */
  int filled_vertices;

  /* the GL mode for drawing this face */
  GLenum draw_mode;

  /* the face's colour */
  float colour[4];

} face;

/* interface function prototypes */
face *alloc_face_array(int );
bool init_face(face *,int);
void free_face(face *);
void add_index(face *,int);
void print_face(face);
face *add_face_to_array(face *,face, int *);
bool join_faces(face *,face);
#endif /* !_CB_FACE_H */
