#ifndef _HV5530_H_
#define _HV5530_H_

#include <stdbool.h>
#include <stdint.h>


typedef void (*write_pin_t)(uint8_t);
typedef void (*delay_t)(uint8_t);

typedef struct hv5530_struct {
    write_pin_t write_clk;
    write_pin_t write_data;
    write_pin_t write_le;
    write_pin_t write_blank;
    delay_t delay_us;
    uint8_t clock_half_cycle;
} hv5530_t;


void shift_bytes(hv5530_t* chip, uint64_t data, uint8_t bits);

void display_number(hv5530_t* chip, float num);

#endif // _HV5530_H_