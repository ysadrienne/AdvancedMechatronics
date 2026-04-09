#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c0
#define I2C_SDA 8
#define I2C_SCL 9

void initI2C();
void setPin(unsigned char, unsigned char, unsigned char);
unsigned char readPin(unsigned char, unsigned char);

int main()
{
    stdio_init_all();
    initI2C();



    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}


void initI2C() {
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

void setPin(unsigned char address, unsigned char register, unsigned char value) {
    i2c_write_blocking(i2c_default, ADDR, buf, 2, false);
}

unsigned char readPin(unsigned char address, unsigned char register) {
    i2c_write_blocking(i2c_default, ADDR, &reg, 1, true); 
    i2c_read_blocking(i2c_default, ADDR, &buf, 1, false);
}