/********************
 * FILE: filereader.c
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Functions for reading a NOFF file. N4OFF files or any other OFF format
 *     are unsupported at the moment.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "object.h"
#include "face.h"
#include "vertex.h"
#include "filereader.h"

/* readFile():
   description: does all the work. see file description
 */
void readfile(object *o, const char *filename){
  int i,j,in;
  int n_vertices,n_faces,n_indices,index;
  vertex v;
  face f;
  FILE *file;
  char str[1024];

  /* Attempt to open the file */
  if((file = fopen(filename,"r"))==NULL){
    fprintf(stderr,"Error: failed to open file %s\n",filename);
    exit(1);
  }

  /* Check this is a NOFF file */
  fscanf(file,"%s",str);
  if(strcmp(str,"NOFF")!=0){
    fprintf(stderr,"Error: file %s is not of NOFF format\n",filename);
    exit(1);
  }

  /* read in the number of vertices, faces and edges */
  fscanf(file,"%d %d %*d",&n_vertices,&n_faces);

  /* init the object model */
  if(init_object(o,n_vertices,n_faces)==false){
    fprintf(stderr,"Error: unsuccessful call to init_object\n");
    exit(1);
  }

  /* load all the vertices */
  for(i = 0; i < o->n_vertices; i++) {
    /* read in a line of vertices*/
    fscanf(file,"%f %f %f %f %f %f",
                &(v.x),&(v.y),&(v.z),&(v.normX),&(v.normY),&(v.normZ));

#ifdef DEBUG
    print_vertex(v);
#endif

    /* store it */
    add_vertex(o,v);
  }

  /* load all the faces */
  for(i = 0; i < o->n_faces; i++) {
    /* read in the number of indices */
    fscanf(file,"%d",&n_indices);

    /* init the face */
    if(init_face(&f,n_indices)==false){
      fprintf(stderr,"Error: unsuccessful call to init_face\n");
      exit(1);
    }

    /* load all the indices*/
    for(j=0; j < n_indices ; j++){
      fscanf(file,"%d",&index);

#ifdef DEBUG
      printf("[%d] = %d ",j,index);
#endif

      add_index(&f,index);
    }
#ifdef DEBUG
    printf("\n");
#endif

    /* grab the rest of the line*/
    fgets(str,1024,file);

    /* zero the alpha value*/
    f.colour[3]=0;

    /* try and load the coloura */
    in = sscanf(str,"%f %f %f %f",
               &f.colour[0],&f.colour[1],&f.colour[2],&f.colour[3]);

    /* if we didn't load any colours set the red value to the default */
    if(in <= 0) {
      f.colour[0] = DEFAULT_COLOUR;
      in=1;
    }

    /* set all the others to the first if only 1 value */
    if(in < 3)
      for(;in<3;in++)
        f.colour[in] = f.colour[0];

#ifdef DEBUG
    print_face(f);
#endif

    /* store it */
    add_face(o,f);
  }

  /* close the file */
  fclose(file);
}
