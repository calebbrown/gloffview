/********************
 * FILE: face.c
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 28-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Functions for handling the 'face' structure, such as creating arrays
 *     of faces, initalising a face, freeing a face and printing a face
 */

#include <stdlib.h> /*for malloc, realloc*/
#include <stdio.h> /*for printf*/
#include <string.h> /*for memcpy*/

#include "common.h"
#include "face.h"

/* alloc_face_array():
   description: allocates an array of faces
   inputs: size of array
   output: pointer to alloced memory or NULL */
face *alloc_face_array(int size){
  return (face *) malloc(sizeof(face) * size);
}


/* alloc_vertex_indicies_array():
   description: initalises a face, ready to have vertex indicies added
   inputs: pointer to an allocated face struct, number of vertex indicies
   output: bool - true on success, false on failure
 */
bool init_face(face *f,int n_vertices){
  if(f == NULL) return false;

  f->filled_vertices = 0;

  /* malloc space for array of vertex indicies */
  f->vertex_indices = (int *) malloc(sizeof(int) * n_vertices);

  if(f->vertex_indices == NULL)
    return false;

  f->n_vertices = n_vertices;

  /* Determine the method we'll use for drawing */
  switch(n_vertices){
    case 3:
      f->draw_mode = GL_TRIANGLES;
      break;
    case 4:
      f->draw_mode = GL_QUADS;
      break;
    default:
      f->draw_mode = GL_POLYGON;
  }

  return true;
}


/* free_face():
   description: frees up a face and all its data
   inputs: pointer to an allocated face
 */
void free_face(face *f){
  if(f == NULL) return;

  free(f->vertex_indices);
}


/* add_index():
   description: adds a vertex index for this face to the  list
   inputs: pointer to an allocated face, the vertex index
 */
void add_index(face *f,int i){
  if(f == NULL) return;

  /* don't add anymore once we've filled up */
  if(f->filled_vertices == f->n_vertices) return;

  f->vertex_indices[f->filled_vertices] = i;

  f->filled_vertices++;

}


/* print_face():
   description: pretty print function for displaying the face
   inputs: a face to print
 */
void print_face(face f){
  printf("f: vertices %d/%d colours (%f,%f,%f,%f)\n",
         f.filled_vertices,
         f.n_vertices,
         f.colour[0],
         f.colour[1],
         f.colour[2],
         f.colour[3]);

}

/* add_face_to_array():
   description: adds a face to an array of faces
   inputs: a pointer to the array of faces,
           the face to add and the size of the array
   outputs: a pointer to the array of faces - maybe different
            to the passed parameter due to realloc.
 */
face *add_face_to_array(face *fd,face fs, int *size) {
  face *new_fd;

  /* increase the size of the memory we are working with */
  (*size) ++;

  /* (re)alloc the memory */
  new_fd = (face *) realloc(fd, (*size) * sizeof(face));

  if(new_fd != NULL) {
    /* add the new face */
    new_fd[(*size)-1] = fs;
  }

  return new_fd;
}

/* join_faces():
   description: takes two faces and 'merges' them together in to
                the destination face, however it doesn't destroy the source face
   inputs: pointer to the destination face, the source face
   outpus: true/false depending on the success of the realloc
 */
bool join_faces(face *fd,face fs){
  int *new_indices;

  /* realloc the space required */
  new_indices = (int *) realloc(fd->vertex_indices,
                         (fs.n_vertices + fd->n_vertices)*sizeof(int));

  /* if the realloc failed return false */
  if(new_indices == NULL)
    return false;

  /* copy in the values into the space */
  memcpy(&(new_indices[fd->n_vertices]),
         fs.vertex_indices,
         sizeof(int)*fs.n_vertices);

  /* update variables*/
  fd->vertex_indices = new_indices;
  fd->n_vertices += fs.n_vertices;

  return true;
}
