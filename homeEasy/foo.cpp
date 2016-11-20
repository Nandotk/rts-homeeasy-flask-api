/**
 **    Home easy foo(s)
 **
 **    @authors nand - 17/11/2016
 **    @version 1.0
 **                                                    _             _
 **       /\  /\___  _ __ ___   ___    /\ /\___  _ __ | |_ _ __ ___ | |
 **      / /_/ / _ \| '_ ` _ \ / _ \  / //_/ _ \| '_ \| __| '__/ _ \| |
 **     / __  / (_) | | | | | |  __/ / __ \ (_) | | | | |_| | | (_) | |
 **     \/ /_/ \___/|_| |_| |_|\___| \/  \/\___/|_| |_|\__|_|  \___/|_|
 **
 **/

#include "foo.h"

void    log(const char *a){
    /** @foo logs */
    cout << a << endl;
}

void    scheduler_realtime() {
    struct sched_param  p;
    p.__sched_priority = sched_get_priority_max(SCHED_RR);
    if( sched_setscheduler( 0, SCHED_RR, &p ) == -1 ) {
        perror("Failed to switch to realtime scheduler.");
    }
}

void    scheduler_standard() {
    struct sched_param  p;
    p.__sched_priority = 0;
    if( sched_setscheduler( 0, SCHED_OTHER, &p ) == -1 ) {
        perror("Failed to switch to normal scheduler.");
    }
}

void    sendBit(bool b) {
    if (b) {
        digitalWrite(GPIO_BASE, HIGH);
        delayMicroseconds(310);
        digitalWrite(GPIO_BASE, LOW);
        delayMicroseconds(1340);
    } else {
        digitalWrite(GPIO_BASE, HIGH);
        delayMicroseconds(310);
        digitalWrite(GPIO_BASE, LOW);
        delayMicroseconds(310);
    }
}

unsigned long   power2(int power) {
    unsigned long   integer;

    integer = 1;
    for (int i = 0; i < power; i++) {
        integer *= 2;
    }

    return (integer);
}

void    itobinary(unsigned long integer, int length, bool *pBoolean) {
    for (int i = 0; i < length; i++) {
        if ((integer / power2(length -1 -i)) == 1) {
            integer -= power2(length -1 -i);
            pBoolean[i] = 1;
        } else pBoolean[i] = 0;
    }
}

void    sendPair(bool b) {
    if(b) {
        sendBit(true);
        sendBit(false);
    } else {
        sendBit(false);
        sendBit(true);
    }
}

void        transmit(bool code) {
    int     i;
    bool    addrBoolean[26];
    bool    iBoolean[4];

    itobinary(12325261, 26, addrBoolean);
    itobinary(1, 4, iBoolean);

    digitalWrite(GPIO_BASE, HIGH);
    delayMicroseconds(275);
    digitalWrite(GPIO_BASE, LOW);
    delayMicroseconds(9900);
    digitalWrite(GPIO_BASE, HIGH);
    delayMicroseconds(275);
    digitalWrite(GPIO_BASE, LOW);
    delayMicroseconds(2675);
    digitalWrite(GPIO_BASE, HIGH);

    for(i = 0; i < 26; i++) {
        sendPair(bit2[i]);
    }

    sendPair(false);
    sendPair(code);

    for(i = 0; i < 4; i++) {
        sendPair(pBoolean[i] != 0);
    }

    digitalWrite(GPIO_BASE, HIGH);
    delayMicroseconds(275);
    digitalWrite(GPIO_BASE, LOW);
}