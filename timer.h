/********************
 * FILE: timer.h
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Header file for timer.c. Defines the prototypes for the
 *     interface functions
 */

#ifndef _CB_TIMER_H
#define _CB_TIMER_H

/* interface function prototypes */
void restart_timer(void);
void stop_timer(void);
struct timeval get_timer(void);
void print_timer(void);

#endif /* !_CB_TIMER_H */
