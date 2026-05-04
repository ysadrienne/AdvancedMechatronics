#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "mpu6050.h"

// I2C defines
// This example will use I2C0 on GPIO8 (SDA) and GPIO9 (SCL) running at 400KHz.
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define BUTTON_PIN 17


int main()
{
    stdio_init_all();

    while (!stdio_usb_connected()) {
        sleep_ms(100);
    }
    
    // printf("USB Connected! Initializing I2C...\n");

    // I2C Initialisation. Using it at 400Khz.
    i2c_init(I2C_PORT, 100 * 1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);

    // printf("Starting Init...\n");
    initMPU();
    // printf("Init Done!\n");

    float ax, ay, az, gx, gy, gz, temp;
    while (true) {
        // printf("Attempting Read...\n");
        readMPU(&ax, &ay, &az, &gx, &gy, &gz, &temp);
        // printf("Read Success!\n");
        float tilt_angle = atan2(ay, az) * 180.0 / M_PI;
        int raw_button = gpio_get(BUTTON_PIN);
        printf("%d,%.2f\n", raw_button, tilt_angle);
        sleep_ms(50);
    }
}
