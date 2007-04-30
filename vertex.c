/********************
 * FILE: vertex.c
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 28-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Functions for handling the 'vertex' structure, such as creating arrays
 *     of vertices, normalization and printing
 */

#include <math.h> /*for sqrt*/
#include <stdio.h> /*for printf*/
#include <stdlib.h> /*for malloc*/

#include "vertex.h"


/* alloc_vertex_array():
   description: allocates an array of vertecies
   inputs: size of array
   output: pointer to alloced memory or NULL */
vertex *alloc_vertex_array(int size){
  return (vertex *) malloc(sizeof(vertex) * size);
}

/* normalize_normal():
   description: normalizes the normal of a vertex
   inputs: a pointer to an alloced pointer
 */
void normalize_normal(vertex *v){
  float normal_length = 0;
  float x,y,z;

  /* Make it a bit easier to play with the variables, as well as improve
     locality a bit*/
  x = v->normX;
  y = v->normY;
  z = v->normZ;

  /* normalize the normal of the vertex */
  normal_length = sqrt((x*x) + (y*y) + (z*z));
  x /= normal_length;
  y /= normal_length;
  z /= normal_length;

  /* store the results */
  v->normX = x;
  v->normY = y;
  v->normZ = z;
}


/* print_vertex():
   description: prints a vertex
   inputs: a vertex to print
 */
void print_vertex(vertex v){
    printf("v(%2.4f,%2.4f,%2.4f) norm=(%2.4f,%2.4f,%2.4f)\n",
            v.x,v.y,v.z,v.normX,v.normY,v.normZ);
}
