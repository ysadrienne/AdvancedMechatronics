#include "as5600.h"


void init_as5600() {
    i2c_init(I2C_PORT, 400*1000);
    
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
}

int read_encoder() {
    uint8_t reg = ANGLE_REG_HIGH;
    uint8_t buffer[2] = {0, 0};
    
    i2c_write_blocking(I2C_PORT, AS5600_ADDR, &reg, 1, true);
    i2c_read_blocking(I2C_PORT, AS5600_ADDR, buffer, 2, false);
    int angle = (buffer[0] << 8) | buffer[1];

    return angle;
}