/**
 *
 * Created by nand on 17/11/2016.
 *
 */

#include <cstdint>
#include "../wiringPi.h"


/** tempo RTS */
#define k_tempo_wakeup_pulse 9415;
#define k_tempo_wakeup_silence 89565;
#define k_tempo_synchro_hw 2416;
#define k_tempo_synchro_hw_min 2416 * 0.7;
#define k_tempo_synchro_hw_max 2416 * 1.3;
#define k_tempo_synchro_sw 4550;
#define k_tempo_synchro_sw_min 4550 * 0.7;
#define k_tempo_synchro_sw_max 4550 * 1.3;
#define k_tempo_half_symbol 604;
#define k_tempo_half_symbol_min 604 * 0.7;
#define k_tempo_half_symbol_max 604 * 1.3;
#define k_tempo_symbol 1208;
#define k_tempo_symbol_min 1208 * 0.7;
#define k_tempo_symbol_max 1208 * 1.3;
#define k_tempo_inter_frame_gap = 30415;


#define GPIO_SOMFY 4


/**********************   Class Abstract RTS Protocol   *****************************/
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

CCodecSomfyRTS::CCodecSomfyRTS()
        : _status(k_waiting_synchro), _cpt_synchro_hw(0), _cpt_bits(0), _previous_bit(0), _waiting_half_symbol(false) {
    for (int i = 0; i < 7; ++i) _payload[i] = 0;
}

bool CCodecSomfyRTS::transmit(int8_t cmd, unsigned long rolling_code, int8_t first) {
    // Construction de la trame claire
    int8_t data[7];
    data[0] = 0xA7;
    data[1] = cmd << 4;
    data[2] = (rolling_code & 0xFF00) >> 8;
    data[3] = rolling_code & 0x00FF;

    /** Mettre ici l'adresse de votre TC ou de la TC simulée */
    data[4] = 0xAB;
    data[5] = 0xCD;
    data[6] = 0xEF;

    // Calcul du checksum
    int8_t cksum = 0;
    for (int i = 0; i < 7; ++i) cksum = cksum ^ (data[i] & 0xF) ^ (data[i] >> 4);
    data[1] = data[1] | (cksum);

    // Obsufscation
    int8_t datax[7];
    datax[0] = data[0];
    for (int i = 1; i < 7; ++i) datax[i] = datax[i - 1] ^ data[i];

    // Emission wakeup, synchro hardware et software
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
        int8_t bit_to_transmit = (datax[i / 8] >> (7 - i % 8)) & 0x01;
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
    delayMicroseconds( k_tempo_inter_frame_gap);
}
/*************************************************************************************/
