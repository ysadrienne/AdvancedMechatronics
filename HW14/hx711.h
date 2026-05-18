#ifndef HX711_H_
#define HX711_H_

#include "pico/stdlib.h"

#define SCK_PIN 16
#define DT_PIN 17
#define clock_time_us 1

void init_hx711();
int32_t read_hx711();

#endif