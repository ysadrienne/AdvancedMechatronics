#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 16
#define I2C_SCL 17
#define LED_PICO 15

bool heartbeat_callback(struct repeating_timer *t);
void picoLEDInit();


int main()
{
    stdio_init_all();
    picoLEDInit();

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    // For more examples of I2C use see https://github.com/raspberrypi/pico-examples/tree/master/i2c

    struct repeating_timer timer;
    add_repeating_timer_ms(1000, heartbeat_callback, NULL, &timer);

    ssd1306_setup();
    ssd1306_clear();
    ssd1306_update();
    
    while (true) {
        ssd1306_drawPixel(10, 20, 1);
        ssd1306_update();
        sleep_ms(1000);
        ssd1306_drawPixel(10, 20, 0);
        ssd1306_update();
        sleep_ms(1000);
    }
}


void picoLEDInit () {
    gpio_init(LED_PICO);
    gpio_set_dir(LED_PICO, GPIO_OUT);
}

bool heartbeat_callback(struct repeating_timer *t) {
    static bool led_state = false;
    gpio_put(LED_PICO, led_state);
    led_state = !led_state;
    return true;
}