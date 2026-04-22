#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "math.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PICO_DEFAULT_SPI_RX_PIN 16
#define PICO_DEFAULT_SPI_SCK_PIN  18
#define PICO_DEFAULT_SPI_TX_PIN 19
#define PIN_CS   17


int main()
{
    stdio_init_all();

    // init spi pins and peripheral
    // init cs pin
    // set cs pin high

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN,  GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    
    cs_select(PIN_CS);
    spi_write_blocking(SPI_PORT, data, 2); // where data is a uint8_t array with length len
    cs_deselect(PIN_CS);
   
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

    float v[100];
    for (int i = 0; i < 100; i++) {
        v[i] = (sine(2 * pi * f * t) + 1)/2 * 3.3;
    }

    while (true) {
        // update DAC
        float t = 0;
        t = t+0.01
        // float voltageA = (sine(2 * pi * f * t) + 1)/2 * 3.3; // f = 2 Hz
        float v = 0;
        setDac(0, voltageA);
        //setDAC(1, voltageB);
        sleep_ms(10);
    }
}

void setDac(int channel, float v) {

    uint8_t data[2];
    data[0] = 0b01110000;
    data[0] = data[0]| ((channel & 0b1) << 7) // put channel in spot
    uint16_t theV = v/3.3 * 1023; // 0b11111111
    data[0] = data[0] | ((theV >> 6) & 0b00001111);
    data[1] = (theV << 2) & 0xFF; // 0b11111100

    cs_select(PIN_CS);
    spi_write_blocking(SPI_PORT, data, 2); // where data is a uint8_t array with length len
    cs_deselect(PIN_CS);

}