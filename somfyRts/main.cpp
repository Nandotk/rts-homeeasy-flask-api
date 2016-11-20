/**
 **    Reverse engineering RTS Somfy on 443,42 mHz frequence
 **
 **    @authors yoyolb - 05/08/2014 and clox - 13/01/2015
 **    @version 1.0
 **    @modifying nand 17/11/2016
 **
 **                                                    _             _
 **       /\  /\___  _ __ ___   ___    /\ /\___  _ __ | |_ _ __ ___ | |
 **      / /_/ / _ \| '_ ` _ \ / _ \  / //_/ _ \| '_ \| __| '__/ _ \| |
 **     / __  / (_) | | | | | |  __/ / __ \ (_) | | | | |_| | | (_) | |
 **     \/ /_/ \___/|_| |_| |_|\___| \/  \/\___/|_| |_|\__|_|  \___/|_|
 **
 **/

#include "wiringPi.h"
#include <iostream>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>
#include <sched.h>
#include <sstream>

#include "somfyRts/CCodecSomfyRTS.cpp"
#include "homeEasy/foo.cpp"

#define GPIO_BASE 7

int main(int argc, char **argv) {

    static          CCodecSomfyRTS codecSomfyRTS;
    unsigned long   rc = 0021;
    int             inByte = 0;
    int8_t          cmd;
    char            inChar = argv[1][0];

    if (setuid(0)) {
        perror("setuid");
        return 1;
    }

    scheduler_realtime();
    log("Demarrage du programme");

    if (wiringPiSetup() == -1) {
        log("Librairie Wiring PI introuvable, veuillez lier cette librairie...");
        return -1;

    }

    pinMode(GPIO_BASE, OUTPUT);
    log("Pin GPIO configure en sortie");

    itob(12325261, 26);
    itobInterruptor(1, 4);

    if (inChar) {
        if (inChar == 'm') {
            cmd = 0x02;
            codecSomfyRTS.transmit(cmd, rc, 2);
            int pmax = 2;
            for (int p = 0; p < pmax; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd monter 0x02");
            rc++;
        }

        if (inChar == 'd') {
            cmd = 0x04;
            codecSomfyRTS.transmit(cmd, rc, 2);
            int pmax = 2;
            for (int p = 0; p < pmax; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd descendre 0x04");
            rc++;
        }

        if (inChar == 's') {
            cmd = 0x01;
            codecSomfyRTS.transmit(cmd, rc, 2);
            int pmax = 2;
            for (int p = 0; p < pmax; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd stop 0x01");
            rc++;
        }

        if (inChar == 'p') {
            cmd = 0x08;
            codecSomfyRTS.transmit(cmd, rc, 2);
            int pmax = 20;
            for (int p = 0; p < pmax; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd programmation 0x08 long (association)");
            rc++;
        }

        if (inChar == 'q') {
            cmd = 0x08;
            codecSomfyRTS.transmit(cmd, rc, 2);
            int pmax = 2;
            for (int p = 0; p < pmax; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd programmation 0x08 court (déassociation)");
            rc++;
        }

        if (inChar == 'a') {
            //system("/etc/lcd/screen -p \"Radio signal ON...\"");
            log("envois du signal ON");
            for (int i = 0; i < 5; i++) {
                transmit(true);
                delay(10);
            }
        }

        if (inChar == 'b') {
            //system("/etc/lcd/screen -p \"Radio signal OFF...\"");
            log("envois du signal OFF");
            for (int i = 0; i < 5; i++) {
                transmit(false);
                delay(10);
            }
        }
    }

    log("fin du programme");
    scheduler_standard();
}

