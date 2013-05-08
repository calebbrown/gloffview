/********************
 * FILE: gloffview.h
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Header file for gloffview.c. Defines some structs for encapsulating the
 *     state and configuration.
 */

#ifndef _CB_GLOFFVIEW_H
#define _CB_GLOFFVIEW_H

#include "common.h"

typedef enum { none , rotate, zoom } motion;

/* state struct. for representing the current state */
typedef struct {
  /* values for dealing with interactive rotation */
  int beginx;
  int beginy;
  int x;
  int y;
  motion type;
  bool motion;
  float curquat[4];
  float lastquat[4];

  /* frame counting variables */
  int  frames;
  axis   rot_axis;
  long int  last_frames;
} state;

/* config struct. for represent the program configuration */
typedef struct {
  render_type type;
  bool back_cull;
  bool trackball;
  bool clock;
  bool fps_dump;
  axis rotate_axis;
  int  rotation_rate;
  int  total_frames;
  int  window_width;
  int  window_height;
  int  time_to_run;
} config;

#endif /* !_CB_GLOFFVIEW_H */
