/********************
 * FILE: object.c
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 28-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Functions for handling the 'object' structure, such as initialisation,
 *     and the addition of vertices.
 */
#include <stdlib.h> /*for free()*/

#include "common.h"
#include "face.h"
#include "vertex.h"
#include "object.h"

/* init_object():
   description: initalises an object. allocates space for the vertex array and
                face array.
   inputs: a pointer to an alloced object, the number of vertices it will take
           and the number of faces for the object
   outputs: true/false depending if the allocs were successful
 */
bool init_object(object *o, int n_vert,int n_faces) {
  if(o == NULL) return false;

  o->n_vertices = n_vert;
  o->n_faces = n_faces;

  o->filled_vertices = 0;
  o->filled_faces = 0;

  o->vertices = alloc_vertex_array(n_vert);

  if(o->vertices == NULL)
    return false;

  o->faces = alloc_face_array(n_faces);
  if(o->faces == NULL)
    return false;

  return true;
}


/* free_object():
   description: frees the memory that that has been alloced inside the object.
                This doesn't free the object itself
   inputs: a pointer to the object to free the memory from
 */
void free_object(object *o) {
  int i;

  if(o == NULL) return;

  /* free up the vertices */
  free(o->vertices);

  /* free up all the arrays of vertex indices for each face*/
  for(i=0;i<o->filled_faces;i++)
    free_face(o->faces+i);

  /* free up all the faces*/
  free(o->faces);
}


/* add_vertex_p():
   description: adds a vertex to the object, given the individual parts of the
                vertex
   inputs: a pointer to an alloced object, the x,y,z parts of the vertex and
           the normal x,y,z for the vertex.
 */
void add_vertex_p(object *o,float x,
                  float y,
                  float z,
                  float normX,
                  float normY,
                  float normZ){
  if(o == NULL) return;
  if(o->filled_vertices == o->n_vertices) return;

  o->vertices[o->filled_vertices].x = x;
  o->vertices[o->filled_vertices].y = y;
  o->vertices[o->filled_vertices].z = z;
  o->vertices[o->filled_vertices].normX = normX;
  o->vertices[o->filled_vertices].normY = normY;
  o->vertices[o->filled_vertices].normZ = normZ;

  o->filled_vertices++;
}


/* add_vertex():
   description: adds a vertex to the object
   inputs: a pointer to an alloced object, the vertex to add
 */
void add_vertex(object *o,vertex v){
  if(o == NULL) return;
  if(o->filled_vertices == o->n_vertices) return;

  o->vertices[o->filled_vertices] = v;

  o->filled_vertices++;
}


/* add_face():
   description: adds a face to the object
   inputs: a pointer to an alloced object, the face to add
 */
void add_face(object *o,face f){
  if(o == NULL) return;
  if(o->filled_faces == o->n_faces) return;

  o->faces[o->filled_faces] = f;

  o->filled_faces++;
}


/* optimise():
   description: "optimises" the object. It places all the indices for each face
     into a common face if they have the same colour and vertices (except for
     a face with more than 4 sides).
     This is supposed to save calls to glDrawElement(). It will only work
     with the 'vertex array' render mode with the INDEX_OPTIMISATION
     preprocessor define set.
     Unfortunately this function takes quite a while to run on complex models,
     with no benifit seen.

   inputs: an alloced object that has been filled with all the data
 */
void optimise(object *o){
  face *old_faces = NULL;
  face *new_faces = NULL;

  int i,j,n_faces = 0;

  old_faces = o->faces;

  /* Optimise everyface */
  for(i=0;i < o->n_faces; i++){

    /* Okay, now figure out whether or not we have a face with the same
       draw_mode (not incl GL_POLYGON) and colour */
    for(j=0;j<n_faces;j++){
      if((old_faces[i].draw_mode != GL_POLYGON) &&
         (new_faces[i].draw_mode == old_faces[i].draw_mode) &&
         (new_faces[i].colour[0] == old_faces[i].colour[0]) &&
         (new_faces[i].colour[1] == old_faces[i].colour[1]) &&
         (new_faces[i].colour[2] == old_faces[i].colour[2]))
        break;
    }

    /* boohoo, nothing similar to this face so copy it to the new face list */
    if(n_faces == j) {
      /* copy the face across */
      new_faces = add_face_to_array(new_faces,old_faces[i],&n_faces);
    } else {
    /* yippee, found a similar face, add my indices to the list! */
      /* copy the indices to the array */
      join_faces(&(new_faces[j]),old_faces[i]);

      /* now weve copied the indicies, free up the old memory */
      free_face(&(old_faces[i]));
    }

  }

  /* Get rid of the old, unoptimised list and replace with the new */
  free(old_faces);
  o->faces = new_faces;
  o->n_faces = n_faces;
}

