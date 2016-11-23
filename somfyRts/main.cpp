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

#include "somfyRTS.h"

using namespace std;

int main(int argc, char **argv) {

    static CCodecSomfyRTS codecSomfyRTS;
    int rc = 21;
    int8_t cmd;
    char inChar = argv[1][0];

    if (setuid(0)) {
        perror("setuid");
        return 1;
    }

    rc = getRollingCode();

    scheduler_realtime();
    log("Processing");

    if (wiringPiSetup() == -1) {
        log("WiringPI Library not found");
        return -1;

    } else {
        pinMode(GPIO_SOMFY, OUTPUT);
        log("GPIO 4 OUTPUT");
    }

    switch (inChar) {
        case 'm':
            cmd = 0x02;
            codecSomfyRTS.transmit(cmd, rc, 2);
            for (int p = 0; p < 2; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd monter 0x02");
            rc++;
            break;

        case 'd':
            cmd = 0x04;
            codecSomfyRTS.transmit(cmd, rc, 2);
            for (int p = 0; p < 2; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd descendre 0x04");
            rc++;
            break;

        case 's':
            cmd = 0x01;
            codecSomfyRTS.transmit(cmd, rc, 2);
            for (int p = 0; p < 2; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd stop 0x01");
            rc++;
            break;

        case 'p':
            cmd = 0x08;
            codecSomfyRTS.transmit(cmd, rc, 2);
            for (int p = 0; p < 20; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd programmation 0x08 long (association)");
            rc++;
            break;

        case 'q':
            cmd = 0x08;
            codecSomfyRTS.transmit(cmd, rc, 2);
            for (int p = 0; p < 2; ++p) {
                codecSomfyRTS.transmit(cmd, rc, 7);
            }

            log("cmd programmation 0x08 court (déassociation)");
            rc++;
            break;
    }

    storeRollingCode(rc);

    log("fin du programme");
    scheduler_standard();
}

