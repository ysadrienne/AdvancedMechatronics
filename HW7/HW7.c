#include <stdio.h>
#include <math.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments
#define SPI_PORT spi0
#define PICO_DEFAULT_SPI_RX_PIN 16
#define PICO_DEFAULT_SPI_SCK_PIN  18
#define PICO_DEFAULT_SPI_TX_PIN 19
#define PIN_CS   17

#define pi 3.141592653589793238

void setDac(int channel, float v);

int sine_i = 0;
int tri_i = 0;
int counter = 0;

static inline void cs_select() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 0);
    asm volatile("nop \n nop \n nop");
}
static inline void cs_deselect() {
    asm volatile("nop \n nop \n nop");
    gpio_put(PIN_CS, 1);
    asm volatile("nop \n nop \n nop");
}

int main()
{
    stdio_init_all();

    // SPI initialisation. This example will use SPI at 1MHz.
    spi_init(SPI_PORT, 1000*1000);
    gpio_set_function(PICO_DEFAULT_SPI_RX_PIN, GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_SCK_PIN,  GPIO_FUNC_SPI);
    gpio_set_function(PICO_DEFAULT_SPI_TX_PIN, GPIO_FUNC_SPI);
    
    gpio_init(PIN_CS);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_put(PIN_CS, 1);
    
    // For more examples of SPI use see https://github.com/raspberrypi/pico-examples/tree/master/spi

    float v_sine[100];
    for (int i = 0; i < 100; i++) {
        v_sine[i] = (sin(2 * pi * i / 100.0f) + 1.0f)/2.0f * 3.3f;
    }

    float v_tri[100];
    for (int i = 0; i < 100; i++) {
        if (i < 50) {
             v_tri[i] += (i / 50.0f) * 3.3f;
        } else {
            v_tri[i] = ((100 - i) / 50.0f) * 3.3f;
        }
    }

    while (true) {
        // update DAC
        setDac(0, v_sine[sine_i]);
        sine_i++;
        if (sine_i >= 100) {
            sine_i = 0;
        }

        if (counter % 2 == 0) {
            setDac(1, v_tri[tri_i]);
            tri_i++;
            if (tri_i >= 100) {
                tri_i = 0;
            }
        }
        counter++;
        sleep_ms(5);
    }
}

void setDac(int channel, float v) {
    uint16_t theV = v/3.3 * 1023; // 0b11111111
    uint8_t data[2];
    
    data[0] = (channel == 0) ? 0x30 : 0xB0;
    data[0] = data[0] | ((theV >> 6) & 0b00001111);
    data[1] = (theV << 2) & 0xFF; // 0b11111100

    cs_select(PIN_CS);
    spi_write_blocking(SPI_PORT, data, 2); // where data is a uint8_t array with length len
    cs_deselect(PIN_CS);
}