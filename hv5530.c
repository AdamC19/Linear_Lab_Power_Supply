#include "hv5530.h"
#include <string.h>
#include "printf.h"


void init_hv5530(hv5530_t* chip){
    chip->write_blank(0); // blank the display
    shift_bytes(chip, 0, 64);
    chip->write_blank(1); // de-blank the display
}

/**
 * @brief data is shifted into the chips MSB first. At the end, LE is pulsed
 * to update the pin registers with the shifted-in value
 * 
 * @param chip pointer to the struct holding the chip info
 * @param data 64-bit integer holding the bits to set the pins to
 */
void shift_bytes(hv5530_t* chip, uint64_t data, uint8_t bits){

    chip->write_le(0); // ensure this is low so funky stuff doesn't happen

    for(uint8_t shift = bits - 1; shift < 255; shift--){
        chip->write_clk(1); // rising edge of clock signal
        chip->write_data((data >> shift) & 1); // change data line
        chip->delay_us(chip->clock_half_cycle); // wait for setup
        chip->write_clk(0); // falling edge of clock, data is shifted into chips
        chip->delay_us(chip->clock_half_cycle);
    }

    // PULSE LATCH ENABLE
    chip->write_le(1);
    chip->delay_us(chip->clock_half_cycle); // wait for setup
    chip->delay_us(chip->clock_half_cycle); // wait for setup
    chip->write_le(0);

}


void display_number(hv5530_t* chip, float num){
    uint64_t buf = 0;
    char s[6];
    snprintf(s, 6, "%.2f", num);
    uint8_t numeral = 0;

    for(uint8_t i = 0; i < 4; i++){
        if(s[i] == 46){
            // decimal point
            buf = buf << 1;
            buf |= 1;
        }else{
            numeral = s[i] - 48; // get a value 0 thru 9
            buf = buf << 10;
            buf |= (1 << numeral);
            buf = buf << 1; // shift by one more to leave space for decimal point
        }

    }
}