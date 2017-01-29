/**
 **    home easy header
 **
 **    @author nandotk
 **/

#ifndef __FOO_H__
# define __FOO_H__

# include <wiringPi.h>
# include <iostream>
# include <stdio.h>
# include <sys/time.h>
# include <time.h>
# include <stdlib.h>
# include <sched.h>

# define GPIO_BASE 7

void log(const char *a);

void scheduler_realtime();

void scheduler_standard();

void sendBit(bool b);

unsigned long power2(int power);

void itobinary(unsigned long integer, int length, bool *pBoolean);

void sendPair(bool b);

void transmit(bool code);

#endif
