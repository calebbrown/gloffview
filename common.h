/********************
 * FILE: common.h
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Header file for everything. Defines some common enums and values
 */

#ifndef _CB_COMMON_H
#define _CB_COMMON_H

#define DEGREESPERREV 360

/* true and false, very useful consts */
typedef enum {false, true} bool;

/* X,Y,Z constants, also very useful */
typedef enum { x, y, z} axis;

/* different rendering types */
typedef enum { normal, display_list, vertex_array} render_type;

#endif /*! _CB_COMMON_H */
