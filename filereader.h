/********************
 * FILE: filereader.h
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Header file for filereader.c. Defines the prototypes for the
 *     interface functions
 */

#ifndef _CB_FILEREAD_H
#define _CB_FILEREAD_H

/* the default colour for obects without one*/
#define DEFAULT_COLOUR 0.6

/* interface function prototypes */
void readfile(object *, const char *);

#endif /*!_CB_FILEREAD_H*/
