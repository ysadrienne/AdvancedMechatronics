#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "ssd1306.h"
#include "mpu6050.h"

int main()
{
    stdio_init_all();
    picoLEDInit();

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 400*1000);

    ssd1306_setup();
    ssd1306_clear();
    ssd1306_update();

    struct repeating_timer timer;
    add_repeating_timer_ms(1000, heartbeat_callback, NULL, &timer);

    // unsigned char who = readPin(MPU6050_ADDR, WHO_AM_I);
    initMPU();

    float ax, ay, az, gx, gy, gz, temp;

    while (true) {   
        /*
        printf("Who: 0x%X\r\n", who);
        sleep_ms(1000);
        */
        readMPU(&ax, &ay, &az, &gx, &gy, &gz, &temp);
        printf("ax: %.2f, ay: %.2f, az: %.2f\r\n", ax, ay, az);
        printf("gx: %.2f, gy: %.2f, gz: %.2f, temp: %.2f\r\n", gx, gy, gz, temp);
        sleep_ms(1000);
    }
}