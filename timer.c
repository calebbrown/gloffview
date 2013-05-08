/********************
 * FILE: timer.c
 * CREATION DATE: 26-8-2003
 * MODIFICATION DATE: 29-8-2003
 * AUTHOR: Caleb Brown
 * DESCRIPTION:
 *     Functions for handling timing. Unfortunately it maybe broken :(
 */

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>
#include <stdio.h>

#include "common.h"
#include "timer.h"

/* globals for storing the time */
static bool running=false;
static struct timeval start_utime;
static struct timeval stop_utime;


/* restart_timer():
   description: stores the start time and sets the timer to running
 */
void restart_timer(void){
  struct rusage usage_stats;

  running = true;
  getrusage(RUSAGE_SELF,&usage_stats);

  start_utime = usage_stats.ru_utime;
}


/* stop_timer():
   description: stores the finish time and sets the timer to stopped
 */
void stop_timer(void){
  struct rusage usage_stats;

  /* only stop a the timer if it has been started */
  if(running == true){
    running = false;
    getrusage(RUSAGE_SELF,&usage_stats);

    stop_utime = usage_stats.ru_utime;
  }
}


/* get_timer():
   description: returns the timeval struct for the current time period
 */
struct timeval get_timer(void){
  struct rusage usage_stats;

  if(running == true){
    getrusage(RUSAGE_SELF,&usage_stats);
  } else {
    usage_stats.ru_utime = stop_utime;
  }


  /* hmm, subtraction is messy :) !!!
     gotta apply primary school technique so we don't end up with negative
     numbers.
   */
  while(usage_stats.ru_utime.tv_usec < start_utime.tv_usec){
  printf("start usec=%ld other usec=%ld\n",
         (long int)start_utime.tv_usec,(long int)usage_stats.ru_utime.tv_usec);
    usage_stats.ru_utime.tv_sec -= 1;     /* move the second */
    usage_stats.ru_utime.tv_usec += 1000000; /* to the usec column */
  }
  usage_stats.ru_utime.tv_sec -= start_utime.tv_sec;
  usage_stats.ru_utime.tv_usec -= start_utime.tv_usec;

  return usage_stats.ru_utime;

}


/* print_timer():
   description: print the current value of the timer
 */
void print_timer(void){
  struct timeval temp;

  temp = get_timer();

  printf("timer: secs=%ld usecs=%ld\n",(long int)temp.tv_sec,(long int)temp.tv_usec);
}
