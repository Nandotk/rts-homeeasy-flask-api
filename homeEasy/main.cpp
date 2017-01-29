/**
 **    home easy main control on 443,92 Mhz
 **
 **    @author nandotk
 **/

#include "homeeasy.h"

using namespace std;

int main(int argc, char **argv) {
    int cmd = atoi(argv[1]);

    if (setuid(0)) {
        perror("setuid");
        return 1;
    }

    scheduler_realtime();

    if (wiringPiSetup() == -1) {
        log("WiringPI Library not found");
        return -1;
    }

    pinMode(GPIO_BASE, OUTPUT);
    log("GPIO 7 OUTPUT");

    if (cmd) {
        //system("/etc/lcd/screen -p \"Radio signal ON...\"");
        log("sending ON HomeEasy ...");
        for (int i = 0; i < 5; i++) {
            transmit(true);
            delay(10);
        }
    } else {
        //system("/etc/lcd/screen -p \"Radio signal OFF...\"");
        log("sending OFF HomeEasy");
        for (int i = 0; i < 5; i++) {
            transmit(false);
            delay(10);
        }
    }
    scheduler_standard();
    return (0);
}

