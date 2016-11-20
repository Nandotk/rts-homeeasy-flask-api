/**
 **    Home easy header
 **    @authors nand - 20/11/2016
 **    @version 1.0
 **                                                    _             _
 **       /\  /\___  _ __ ___   ___    /\ /\___  _ __ | |_ _ __ ___ | |
 **      / /_/ / _ \| '_ ` _ \ / _ \  / //_/ _ \| '_ \| __| '__/ _ \| |
 **     / __  / (_) | | | | | |  __/ / __ \ (_) | | | | |_| | | (_) | |
 **     \/ /_/ \___/|_| |_| |_|\___| \/  \/\___/|_| |_|\__|_|  \___/|_|
 **
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
# include <sstream>

# define GPIO_BASE 7

void            log(const char *a);
void            scheduler_realtime();
void            scheduler_standard();
void            sendBit(bool b);
unsigned long   power2(int power);
void            itobinary(unsigned long integer, int length, bool *pBoolean);
void            sendPair(bool b);
void            transmit(bool code);

#endif
