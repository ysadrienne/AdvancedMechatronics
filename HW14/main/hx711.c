#include "hx711.h"
#include "pico/stdlib.h"

void init_hx711() {
    gpio_init(SCK_PIN);
    gpio_set_dir(SCK_PIN, GPIO_OUT);
    gpio_put(SCK_PIN, 0);

    gpio_init(DT_PIN);
    gpio_set_dir(DT_PIN, GPIO_IN);
    gpio_pull_up(DT_PIN);
}

int32_t read_hx711() {
    while (gpio_get(DT_PIN)) {
        tight_loop_contents(); 
    }

    uint32_t raw = 0;
    for (int i = 0; i < 24; i++) {
        gpio_put(SCK_PIN, true);
        sleep_us(clock_time_us);

        raw = raw << 1;
        if (gpio_get(DT_PIN)) {
            raw |= 1;
        }
        gpio_put(SCK_PIN, 0);
        sleep_us(clock_time_us);
    }

    // 25th pulse to set gain = 128 for next reading
    gpio_put(SCK_PIN, 1);
    sleep_us(clock_time_us);
    gpio_put(SCK_PIN, 0);
    sleep_us(clock_time_us);

    int32_t signed_raw = (int32_t)raw;
    if (signed_raw & 0x800000) {
        signed_raw |= 0xFF000000;
    }

    return (int)signed_raw;
}