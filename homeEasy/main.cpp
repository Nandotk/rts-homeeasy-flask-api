/**
 **    Home easy main control on 443,92 mHz frequence
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

#include "foo.h"

using namespace std;

int main(int argc, char **argv) {
    char *cmd = argv[1];

    if (setuid(0)) {
        perror("setuid");
        return 1;
    }

    scheduler_realtime();
    log("Processing");

    if (wiringPiSetup() == -1) {
        log("WiringPI Library not found");
        return -1;
    }

    pinMode(GPIO_BASE, OUTPUT);
    log("GPIO 7 OUTPUT");

    if (cmd) {
        if (cmd == "on") {
            //system("/etc/lcd/screen -p \"Radio signal ON...\"");
            log("sending ON HomeEasy ...");
            for (int i = 0; i < 5; i++) {
                transmit(true);
                delay(10);
            }
        } else if (cmd == "off") {
            //system("/etc/lcd/screen -p \"Radio signal OFF...\"");
            log("sending OFF HomeEasy");
            for (int i = 0; i < 5; i++) {
                transmit(false);
                delay(10);
            }
        } else {
            log("Usage : .[binary] [ON/OFF]");
        }
    }

    log("Ends");
    scheduler_standard();
}

