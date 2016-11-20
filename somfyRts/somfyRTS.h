/**
 **    header RTS Somfy
 **
 **    @authors nand 17/11/2016
 **                                                    _             _
 **       /\  /\___  _ __ ___   ___    /\ /\___  _ __ | |_ _ __ ___ | |
 **      / /_/ / _ \| '_ ` _ \ / _ \  / //_/ _ \| '_ \| __| '__/ _ \| |
 **     / __  / (_) | | | | | |  __/ / __ \ (_) | | | | |_| | | (_) | |
 **     \/ /_/ \___/|_| |_| |_|\___| \/  \/\___/|_| |_|\__|_|  \___/|_|
 **
 **/

#ifndef __SOMFY_RTS_H__
# define __SOMFY_RTS_H__

# include <cstdint>
# include <wiringPi.h>
# include <iostream>
# include <stdio.h>
# include <sys/time.h>
# include <time.h>
# include <stdlib.h>
# include <sched.h>
# include <sstream>
# include <fstream>

# define GPIO_SOMFY 4

/** tempo RTS */
# define k_tempo_wakeup_pulse 9415;
# define k_tempo_wakeup_silence 89565;
# define k_tempo_synchro_hw 2416;
# define k_tempo_synchro_hw_min 2416 * 0.7;
# define k_tempo_synchro_hw_max 2416 * 1.3;
# define k_tempo_synchro_sw 4550;
# define k_tempo_synchro_sw_min 4550 * 0.7;
# define k_tempo_synchro_sw_max 4550 * 1.3;
# define k_tempo_half_symbol 604;
# define k_tempo_half_symbol_min 604 * 0.7;
# define k_tempo_half_symbol_max 604 * 1.3;
# define k_tempo_symbol 1208;
# define k_tempo_symbol_min 1208 * 0.7;
# define k_tempo_symbol_max 1208 * 1.3;
# define k_tempo_inter_frame_gap 30415;


class CCodecSomfyRTS {

public:
    enum t_status {
        k_waiting_synchro
    };

public:
    CCodecSomfyRTS();

    bool transmit(int8_t cmd, unsigned long rolling_code, int8_t first);

protected:
    t_status _status;
    int8_t _cpt_synchro_hw;
    int8_t _cpt_bits;
    int8_t _previous_bit;
    bool _waiting_half_symbol;
    int8_t _payload[7];

};

void log(const char *a);

void scheduler_realtime();

void scheduler_standard();

int getRollingCode();

void storeRollingCode(int rcode);

#endif
