/**
 **    Class RTS Somfy
 **
 **    @authors nand 17/11/2016
 **                                                    _             _
 **       /\  /\___  _ __ ___   ___    /\ /\___  _ __ | |_ _ __ ___ | |
 **      / /_/ / _ \| '_ ` _ \ / _ \  / //_/ _ \| '_ \| __| '__/ _ \| |
 **     / __  / (_) | | | | | |  __/ / __ \ (_) | | | | |_| | | (_) | |
 **     \/ /_/ \___/|_| |_| |_|\___| \/  \/\___/|_| |_|\__|_|  \___/|_|
 **
 **/

#include "somfyRTS.h"

void log(const char *a) {
    /** @foo logs */
    cout << a << endl;
}

void scheduler_realtime() {
    struct sched_param p;
    p.__sched_priority = sched_get_priority_max(SCHED_RR);
    if (sched_setscheduler(0, SCHED_RR, &p) == -1) {
        perror("Failed to switch to realtime scheduler.");
    }
}

void scheduler_standard() {
    struct sched_param p;
    p.__sched_priority = 0;
    if (sched_setscheduler(0, SCHED_OTHER, &p) == -1) {
        perror("Failed to switch to normal scheduler.");
    }
}

int getRollingCode () {
    int rcode;
    char *line;
    ifstream rc ("rc.txt");
    if (rc.is_open())
    {
        while ( getline (rc, line) )
        {
            rcode = atoi(line);
        }
        rc.close();
    } else {
        log("Unable to open file");
    }
}

void storeRollingCode() {
    ofstream myfile ("example.txt");
    if (myfile.is_open())
    {
        myfile << "This is a line.\n";
        myfile << "This is another line.\n";
        myfile.close();
    }
    else cout << "Unable to open file";
}

/** init class payload to 0 */
CCodecSomfyRTS::CCodecSomfyRTS()
        : _status(k_waiting_synchro), _cpt_synchro_hw(0), _cpt_bits(0), _previous_bit(0), _waiting_half_symbol(false) {
    for (int i = 0; i < 7; ++i) _payload[i] = 0;
}

/** transmit function that follow RTS */
bool CCodecSomfyRTS::transmit(int8_t cmd, unsigned long rolling_code, int8_t first) {

    /** Construction de la trame claire */
    int8_t data[7];
    data[0] = (int8_t) 0xA7;
    data[1] = cmd << 4;
    data[2] = (int8_t) ((rolling_code & 0xFF00) >> 8);
    data[3] = (int8_t) (rolling_code & 0x00FF);

    /** Mettre ici l'adresse de votre TC ou de la TC simulée */
    data[4] = (int8_t) 0xAB;
    data[5] = (int8_t) 0xCD;
    data[6] = (int8_t) 0xEF;

    /** Calcul du checksum */
    int8_t cksum = 0;
    for (int i = 0; i < 7; ++i) cksum = (int8_t) (cksum ^ (data[i] & 0xF) ^ (data[i] >> 4));
    data[1] = data[1] | (cksum);

    /** Obsufscation */
    int8_t datax[7];
    datax[0] = data[0];
    for (int i = 1; i < 7; ++i) datax[i] = datax[i - 1] ^ data[i];

    /** Emission wakeup, synchro hardware et software */
    digitalWrite(GPIO_SOMFY, 1);
    delayMicroseconds(k_tempo_wakeup_pulse);
    digitalWrite(GPIO_SOMFY, 0);
    delayMicroseconds(k_tempo_wakeup_silence);

    for (int i = 0; i < first; ++i) {
        digitalWrite(GPIO_SOMFY, 1);
        delayMicroseconds(k_tempo_synchro_hw);
        digitalWrite(GPIO_SOMFY, 0);
        delayMicroseconds(k_tempo_synchro_hw);
    }

    digitalWrite(GPIO_SOMFY, 1);
    delayMicroseconds(k_tempo_synchro_sw);
    digitalWrite(GPIO_SOMFY, 0);
    delayMicroseconds(k_tempo_half_symbol);

    /** Emission des donnees */
    for (int i = 0; i < 56; ++i) {
        int8_t bit_to_transmit = (int8_t) (datax[i / 8] >> (7 - i % 8) & 0x01);
        if (bit_to_transmit == 0) {
            digitalWrite(GPIO_SOMFY, 1);
            delayMicroseconds(k_tempo_half_symbol);
            digitalWrite(GPIO_SOMFY, 0);
            delayMicroseconds(k_tempo_half_symbol);
        } else {
            digitalWrite(GPIO_SOMFY, 0);
            delayMicroseconds(k_tempo_half_symbol);
            digitalWrite(GPIO_SOMFY, 1);
            delayMicroseconds(k_tempo_half_symbol);
        }
    }

    digitalWrite(GPIO_SOMFY, 0);
    delayMicroseconds(k_tempo_inter_frame_gap);
}