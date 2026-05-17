#include <stdio.h>
#include "pico/stdlib.h"

#define SCK_PIN 16
#define DT_PIN 17

void init_gpios(void);
int32_t read_hx711(void);

int main()
{
    stdio_init_all();
    init_gpios();

    while (true) {
        int32_t force_val = read_hx711();
        printf("Raw Force Value: %d\n", force_val);
        sleep_ms(50);
    }
}

void init_gpios() {
    gpio_init(SCK_PIN);
    gpio_set_dir(SCK_PIN, GPIO_OUT);
    gpio_put(SCK_PIN, false);
    gpio_init(DT_PIN);
    gpio_set_dir(DT_PIN, GPIO_IN);
}

int32_t read_hx711(void) {
    uint32_t raw = 0;

    while (gpio_get(DT_PIN)) {
        tight_loop_contents(); 
    }

    for (int i = 0; i < 24; i++) {
        gpio_put(SCK_PIN, true);
        sleep_us(1);

        raw = raw << 1;
        if (gpio_get(DT_PIN)) {
            raw |= 1;
        }

        gpio_put(SCK_PIN, false);
        sleep_us(1);
    }

    gpio_put(SCK_PIN, true);
    sleep_us(1);
    gpio_put(SCK_PIN, false);
    sleep_us(1);

    int32_t signed_raw = (int32_t)raw;
    if (signed_raw & 0x800000) {
        signed_raw |= 0xFF000000;
    }

    return signed_raw;
}