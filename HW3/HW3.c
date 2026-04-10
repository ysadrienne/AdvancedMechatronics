#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9
#define LED_PICO 16
#define ADDR 0x20
#define IODIR 0x00
#define GPIO_REG 0x09
#define OLAT 0x0A

void picoLEDInit();
void initI2C();
void initMCP();
bool heartbeat_callback(struct repeating_timer *t);
void setPin(unsigned char, unsigned char, unsigned char);
unsigned char readPin(unsigned char, unsigned char);

int main()
{
    stdio_init_all();
    picoLEDInit();
    initI2C();
    initMCP();

    struct repeating_timer timer;
    add_repeating_timer_ms(1000, heartbeat_callback, NULL, &timer);

    while (true) {
        unsigned char status = readPin(ADDR, GPIO_REG);
        if (!(status & 0x01)) {
            setPin(ADDR, OLAT, 0x80);
        }
        else {
            setPin(ADDR, OLAT, 0x00);
        }
        sleep_ms(100);
    }
}


void picoLEDInit () {
    gpio_init(LED_PICO);
    gpio_set_dir(LED_PICO, GPIO_OUT);
}

void initI2C() {
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

bool heartbeat_callback(struct repeating_timer *t) {
    static bool led_state = false;
    gpio_put(LED_PICO, led_state);
    led_state = !led_state;
    return true;
}

void initMCP() {
    uint8_t setup[2];
    setup[0] = IODIR;
    setup[1] = 0x7F;
    i2c_write_blocking(I2C_PORT, ADDR, setup, 2, false);
}

void setPin(unsigned char address, unsigned char reg, unsigned char value) {
    uint8_t buf[2];
    buf[0] = reg;
    buf[1] = value;
    i2c_write_blocking(I2C_PORT, address, buf, 2, false);
}

unsigned char readPin(unsigned char address, unsigned char reg) {
    uint8_t value;
    i2c_write_blocking(I2C_PORT, address, &reg, 1, true); 
    i2c_read_blocking(I2C_PORT, address, &value, 1, false);
    return value;
}