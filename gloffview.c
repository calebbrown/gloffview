/********************
 * FILE: gloffview.c
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     gloffview loads and displays .off files (NOFF only at the moment).
 * PARAMETERS:
 *     r [x|y|z] - rotation axis. the axis to rotate about
 *     f x       - total frames to render, where 'x' is the number of frames
 *     w x       - window width & height, where 'x' is the number of pixels
 *     a x       - degrees to rotate the model per frame
 *     b         - turn ON back face culling
 *     o [n|d|v] - optimisation mode. n = normal, no optimisation
 *                                    d = use display lists
 *                                    v = use vertex arrays
 *     t         - track ball mode. Interactive rotation of the model.
 *                 'r','f','w','a' parameters have no effect when 't'
 *                 is specified as a parameter.
 *
 *     c x       - clocked mode. Run for x seconds and quit, displaying
 *                 fps information.
 *     d x       - fps dump mode. Dump the fps every x seconds.
 */

#include <signal.h>
#include <unistd.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#include "common.h"
#include "gloffview.h"
#include "vertex.h"
#include "face.h"
#include "object.h"
#include "filereader.h"
#include "render.h"
#include "trackball.h"
#include "timer.h"

/* options that we except from the command line
   see getopt manpage for details */
#define opt_string "+br:o:w:f:a:tc:d:"

/* Default options */
#define DEFAULT_WIDTH 400
#define DEFAULT_ROTATION y
#define DEFAULT_BACKFACECULL false
#define DEFAULT_TOTALFRAMES 360
#define DEFAULT_TRACKBALL false
#define DEFAULT_ROTATIONRATE 1
#define DEFAULT_RENDERTYPE normal

#define DEFAULT_CLOCK false
#define DEFAULT_FPS_DUMP false

/* Globals for storing the current state and the configuration */
state current;
config options;


void fps_output(int sig){
  printf("FPS = %f\n",(current.last_frames / ((float)options.time_to_run)));
  current.last_frames = 0;

  if(options.fps_dump)
    alarm(options.time_to_run);
  else if(options.clock) {
    exit(0);
  }
}


/* automatic_idle:
   description: idle callback for non interactive mode. simply rotates the
     model around the desired axis and quits at the appropriate time
 */
void automatic_idle(void){
  /*struct timeval tv;*/

  /* If we've got to the end then quit unless we're running on a timeout */
  if(current.frames == options.total_frames && !options.clock) {
    /* output profiling information - time and fps */
/*    print_timer();
    tv = get_timer();
    printf("avg fps=%f\n",
           (options.total_frames /
            ( ( ((float)tv.tv_sec))+(tv.tv_usec/1000000.0f)) ) );*/
    exit(0);
  }

  current.angle =
         ((int)(current.angle + options.rotation_rate)) % DEGREESPERREV;

  /* update the rotation */
  set_rotation(current.angle,options.rotate_axis);

  /* increase the total number of frames drawn count */
  current.frames++;

  glutPostRedisplay();
}


/* interactive_key:
   description: callback for trackball mode that handles keypresses.
     For the moment it simply reacts to 'q' by quiting
 */
void interactive_key(unsigned char key, int x,int y){
  if(key == 'q') exit(0);
}


/* interactive_mouse:
   description: callback for trackball mode that handles mouse clicking.
     A left click starts the rotation code.
 */
void interactive_mouse(int button,int state,int x, int y){
  if((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN)){
    current.motion = true;
    current.beginx = x;
    current.beginy = y;
  }
  if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
    current.motion = false;
  }
}


/* interactive_motion:
   description: callback for trackball mode that handles movement of the mouse.
     It updates the rotational quaternion based upon the mouse movement
   inputs: current x and y locations of the mouse
 */
void interactive_motion(int x,int y){
  int w,h;

  /* Set the height and width to easy to use local variables */
  w = options.window_width;
  h = options.window_height;

  /* If were moving then update the quaternion and redisplay */
  if(current.motion == true){
    trackball(current.lastquat,
      (2.0 * current.beginx - w) / w,
      (h - 2.0 * current.beginy) / h,
      (2.0 * x - w) / w,
      (h - 2.0 * y) / h
      );

    current.beginx = x;
    current.beginy = y;

    add_quats(current.lastquat, current.curquat, current.curquat);

    set_quaternion(current.curquat);

    glutPostRedisplay();
  }

}


/* display():
   description: callback for drawing the screen. pretty much just calls render
 */
void display(void){
  current.last_frames++;

  render();
}


/* reshape():
   description: callback for handling window reshaping, calls rendering rezise
   inputs: new window width and new window height
 */
void reshape(int width, int height){
  options.window_width = width;
  options.window_height = height;

  resize(width,height);
}


/**********************
 *** MAIN function  ***
 **********************/
int main(int argc, char *argv[]) {
  int option=0;
  object model;

  /* Setup the defaults */
  options.back_cull = DEFAULT_BACKFACECULL;
  options.rotate_axis = DEFAULT_ROTATION;
  options.window_width = DEFAULT_WIDTH;
  options.window_height = DEFAULT_WIDTH;
  options.total_frames = DEFAULT_TOTALFRAMES;
  options.trackball = DEFAULT_TRACKBALL;
  options.rotation_rate = DEFAULT_ROTATIONRATE;
  options.type = DEFAULT_RENDERTYPE;
  options.clock = DEFAULT_CLOCK;
  options.fps_dump = DEFAULT_FPS_DUMP;

  glutInit(&argc,argv);

  /* Parse the arguments */
  while(option!=-1){
    option = getopt(argc,argv,opt_string);

    switch (option){
      case 'b': /* back face cull */
          options.back_cull = true;
        break;
      case 'r': /* (auto) rotation axis */
        switch (optarg[0]){
          case 'x':
            options.rotate_axis = x;
            break;
          case 'y':
            options.rotate_axis = y;
            break;
          case 'z':
            options.rotate_axis = z;
            break;
          default:
            fprintf(stderr,"Error: invalid option for rotation axis\n");
            break;
        }
        break;
      case 'o': /* rendering type */
        switch (optarg[0]){
          case 'n':
            options.type = normal;
            break;
          case 'd':
            options.type = display_list;
            break;
          case 'v':
            options.type = vertex_array;
            break;
          default:
            fprintf(stderr,"Error: invalid option for o\n");
            exit(1);
        }
        break;
      case 'w': /* window width, assumed square */
        options.window_width = atoi(optarg);
        options.window_height = options.window_width;

        if(options.window_width < 0) {
          fprintf(stderr,
            "Error: please specify a positive integer for window width\n");
          exit(1);
        }

        break;
      case 'f': /* total number of frames (auto) */
        options.total_frames = atoi(optarg);

        if(options.total_frames < 0){
          fprintf(stderr,
            "Error: please specify a positive integer for total frames\n");
          exit(1);
        }

        break;
      case 'a': /* degrees to rotate per frame (auto) */
        options.rotation_rate = atoi(optarg);

        break;
      case 't': /* interactive trackball mode */
        options.trackball = true;
        options.clock = false;
        break;

      case 'd': /* fps DUMP mode */
        options.clock = false;
        options.fps_dump = true;

        options.time_to_run = atoi(optarg);

        if(options.time_to_run < 0) {
          fprintf(stderr,
            "Error: please specify a positive integer for fps dump period\n");
          exit(1);
        }
        break;
      case 'c': /* clock mode */
        if(!options.trackball){
          options.clock = true;
          options.fps_dump = false;

          options.time_to_run = atoi(optarg);

          if(options.time_to_run < 0) {
            fprintf(stderr,
              "Error: please specify a positive integer for clock time\n");
            exit(1);
          }
        }

        break;

    }
  }
  /* Attempt to get the filename index in argv */
  option = optind;

  /* Hmm, its the end of argv. Most'nt of specified a filename. Suicide. */
  if(option == argc){
    fprintf(stderr,"Error: no filename specified\n");
    exit(1);
  }

#ifdef DEBUG
  printf("backface cull = %d\n",options.back_cull);
  printf("trackball =  %d\n",options.trackball);
  printf("rotation axis = %d\n",options.rotate_axis);
  printf("window width = %d\n",options.window_width);
  printf("total frames = %d\n",options.total_frames);
  printf("rotation rate = %d\n",options.rotation_rate);
  printf("render type = %d\n",options.type);
  printf("filename = %s\n",argv[option]);
#endif

  /* Load the model from specified file */
  readfile(&model,argv[option]);

  /* Setup the output with GLUT */
  glutInitWindowSize(options.window_width,options.window_height);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("GLOffView");

  /* Initalise the render */
  init_render(&model,options.back_cull,options.type,
              options.window_width,options.window_height);

  /* Setup common callback functions */
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  /* setup fps output signal if specified */
  if(options.fps_dump || options.clock){
    current.last_frames = 0;
    signal(SIGALRM,fps_output);
    alarm(options.time_to_run);
  }

  printf("Vendor: %s\nRenderer: %s\nVersion: %s\nExtentions: %s\n",
         glGetString(GL_VENDOR),
         glGetString(GL_RENDERER),
         glGetString(GL_VERSION),
         glGetString(GL_EXTENSIONS));

  /* Setup specific callback functions and do other prepwork */
  if(options.trackball) {
    trackball(current.curquat, 0.0, 0.0, 0.0, 0.0);
    glutMouseFunc(interactive_mouse);
    glutMotionFunc(interactive_motion);
    glutKeyboardFunc(interactive_key);

    set_quaternion(current.curquat);

  } else {
    current.frames = 0;
    current.angle = 0;
    glutIdleFunc(automatic_idle);
    glutKeyboardFunc(NULL);

  }

  /* start the timer and go -> */
  /*restart_timer();*/
  glutMainLoop();

  return 0;
}
