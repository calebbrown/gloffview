/********************
 * FILE: vertex.h
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Header file for vertex.c. Defines the vertex structure and contains the
 *     prototypes for the interface functions
 */

#ifndef _CB_VERTEX_H
#define _CB_VERTEX_H

/* vertex struct. stores vertex information

 WARNING!!! - CHANGING the ORDER or TYPE, or ADDING new fields WILL BREAK the
              whole program! */
typedef struct {
  /* vertex normal. these are first for vertex array.
     because we are using GL_N3F_V3F these have to come first.
   */
  float normX;
  float normY;
  float normZ;

  /* the vertex itself! */
  float x;
  float y;
  float z;

} vertex;

/* interface function prototypes */
vertex *alloc_vertex_array(int);
void normalize_normal(vertex *);
void print_vertex(vertex);
#endif /* !_CB_VERTEX_H */
