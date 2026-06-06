#ifndef AS5600_H_
#define AS5600_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include "pico/stdlib.h"
#include "hardware/i2c.h"

// registers for i2c
#define I2C_PORT i2c0
#define I2C_SDA 16
#define I2C_SCL 17
#define LED_PICO 15
#define AS5600_ADDR 0x36

// data registers
#define ANGLE_REG_HIGH 0x0E
#define ANGLE_REG_LOW 0x0F

// function prototype
void init_as5600();
int read_encoder();
#endif