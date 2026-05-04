#include <string.h> // for memset
#include "hardware/i2c.h"
#include "pico/stdlib.h"
#include "mpu6050.h"

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

void initMPU() {
    setPin(MPU6050_ADDR, PWR_MGMT_1, 0x00);
    setPin(MPU6050_ADDR, ACCEL_CONFIG, 0x00);
    setPin(MPU6050_ADDR, GYRO_CONFIG, (3 << 3));
}

void readMPU(float *ax, float *ay, float *az, float *gx, float *gy, float *gz, float *temp) {
    uint8_t buffer[14];
    uint8_t start = 0x3B;

    i2c_write_blocking(I2C_PORT, MPU6050_ADDR, &start, 1, true);
    i2c_read_blocking(I2C_PORT, MPU6050_ADDR, buffer, 14, false);

    int16_t raw_ax = (buffer[0] << 8) | buffer[1];
    int16_t raw_ay = (buffer[2] << 8) | buffer[3];
    int16_t raw_az = (buffer[4] << 8) | buffer[5];
    int16_t raw_temp = (buffer[6] << 8) | buffer[7];
    int16_t raw_gx = (buffer[8] << 8) | buffer[9];
    int16_t raw_gy = (buffer[10] << 8) | buffer[11];
    int16_t raw_gz = (buffer[12] << 8) | buffer[13];

    *ax = raw_ax * 0.000061f;
    *ay = raw_ay * 0.000061f;
    *az = raw_az * 0.000061f;
    *gx = raw_gx * 0.007630f;
    *gy = raw_gy * 0.007630f;
    *gz = raw_gz * 0.007630f;
    *temp = raw_temp / 340.00 + 36.53;
}